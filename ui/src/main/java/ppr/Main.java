package ppr;

import com.fasterxml.jackson.databind.ObjectMapper;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Pos;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import ppr.messages.*;
import ppr.util.Input;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.List;
import java.util.stream.Collectors;


/**
 * @author Kacper Urbaniec
 * @version 2023-03-24
 */
public class Main  extends  Application{
    private static int size = 10;
    private static int generationCount = 0;
    private static GridPane grid = new GridPane();
    private static Label generationLabel = new Label();
    private static int width = 600;
    private static int height = 600;

    public static void main(String[] args) {

        StartMessage start;
        String startJson = "";
        var objectMapper = new ObjectMapper();
        {
            var textInput = ".........." +
                    "...**....." +
                    "....*....." +
                    ".........." +
                    ".........." +
                    "...**....." +
                    "..**......" +
                    ".....*...." +
                    "....*....." +
                    "..........";
            var grid = Input.textInput(textInput);


            start = new StartMessage();
            var input = new GameInput();
            input.size = size;
            input.grid = grid;
            start.input = input;

            // =================
            //
            // DETERMINES IF INTERACTIVE OR BENCHMARK MODE IS EXECUTED
            // * value is null => INTERACTIVE
            // * value not null => BENCHMARK
            //
            // =================
            // var benchmarkInput = new BenchmarkInput();
            // benchmarkInput.generations = 10;
            // benchmarkInput.iterations = 10;
            // start.benchmarkInput = benchmarkInput;

            // Customize thread config for server
            // var threadConfig = new ThreadConfig();
            // threadConfig.dynamic = false;
            // threadConfig.threadCount = 1;
            // start.threadConfig = threadConfig;

            try {
                startJson = objectMapper.writeValueAsString(start);
                System.out.println(startJson);
                System.out.println();
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }


        String hostname = "localhost";
        int portNumber = 1234;
        var grid = start.input.grid;
        try (
                var socket = new Socket(hostname, portNumber);
                var out = new PrintWriter(socket.getOutputStream(), true);
                var in = new BufferedReader(new InputStreamReader(socket.getInputStream()))
        ) {
            // TODO: exit? error handling?

            if (start.benchmarkInput == null) {
                Thread launchThread = new Thread(() -> launch(args));
                launchThread.start();
                interactive(startJson, grid, 10, out, in);
            }
            else
                benchmark(startJson, grid, out, in);

        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public static void interactive(
        String startJson,
        List<List<Boolean>> grid,
        int generations,
        PrintWriter out,
        BufferedReader in
    ) throws IOException, InterruptedException {
        var objectMapper = new ObjectMapper();

        out.println(startJson);

        for (var i = 0; i < generations; ++i) {
            // Get diff
            var updateJson = in.readLine();
            var update = objectMapper.readValue(updateJson, UpdateMessage.class);

            update.updateGrid(grid);
            // Visualize grid
            String gridString = gridString(grid);
            createChessboard(grid);
            Platform.runLater(() -> {
                generationLabel.setText("Generation: " + update.generation);
            });
            System.out.println(update.generation);
            System.out.println(gridString);

            // Wait for a bit
            Thread.sleep(500);

            // Send if processing should continue or not
            boolean next = (i + 1) != generations;
            var ackMessage = new UpdateAckMessage(next);
            var ackMessageJson = objectMapper.writeValueAsString(ackMessage);
            out.println(ackMessageJson);
        }
    }

    public static void benchmark(
            String startJson,
            List<List<Boolean>> grid,
            PrintWriter out,
            BufferedReader in
    ) throws IOException, InterruptedException {
        var objectMapper = new ObjectMapper();

        out.println(startJson);

        var resultJson = in.readLine();
        var result = objectMapper.readValue(resultJson, BenchmarkMessage.class);

        System.out.println("Average time: " + result.averageTime + " ms");
        System.out.println("Min time: " + result.minTime + " ms");
        System.out.println("Max time: " + result.maxTime + " ms");
        String gridString = gridString(result.grid);
        System.out.println(gridString);
    }

    public static String gridString(List<List<Boolean>> grid) {
        return grid.stream()
                .map(n -> n
                        .stream()
                        .map(n2 -> {
                            if (n2) return "1";
                            else return "0";
                        })
                        .collect(Collectors.joining(" ", "", "\n"))
                )
                .collect(Collectors.joining(""));
    }

    @Override
    public void start(Stage primaryStage) {
        // Create the chessboard
        createChessboard(null);

        // Create the Start and Next buttons
        Button startButton = new Button("Start");
        Button nextButton = new Button("Next");
        startButton.setOnAction(e -> handleStartButtonClick());
        nextButton.setOnAction(e -> handleNextButtonClick());

        // Create the generation counter label
        generationLabel = new Label("Generation: " + generationCount);

        // Create the layout
        GridPane layout = new GridPane();
        layout.setAlignment(Pos.CENTER);
        layout.add(grid, 0, 0, 3, 1);
        layout.add(startButton, 0, 1);
        layout.add(nextButton, 1, 1);
        layout.add(generationLabel, 2, 1);

        // Create the scene and show the stage
        Scene scene = new Scene(layout, width, height);
        primaryStage.setTitle("Game Of Life");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private static void createChessboard(List<List<Boolean>> input) {
        Platform.runLater(() -> {
            grid.getChildren().clear();
        });
        boolean isEmpty = false;
        if(input == null || input.isEmpty()) {
            isEmpty = true;
        } else {
        }
        int rowSize = isEmpty ? size : input.size();
        int recWidth = (width-200)/rowSize;
        for (int row = 0; row < rowSize; row++) {
            int colSize = isEmpty ? size : input.get(row).size();
            int colHeight = (height-200)/colSize;
            for (int col = 0; col < colSize; col++) {
                Boolean value = (row + col) % 2 == 0;
                if(!isEmpty) {
                    value = input.get(row).get(col);
                }
                Rectangle rect = new Rectangle(recWidth, colHeight);
                if (!value) {
                    rect.setFill(Color.WHITE);
                } else {
                    rect.setFill(Color.BLACK);
                }
                final int x = col;
                final int y = row;
                rect.setOnMouseClicked(e -> handleChessboardClick(x, y));
                Platform.runLater(() -> {
                    grid.add(rect, x, y);
                });
            }
        }
    }

    private static void handleChessboardClick(int x, int y) {
        // Handle the click on the chessboard
        System.out.println("Clicked on cell " + x + "," + y);
    }

    private static void handleStartButtonClick() {
        // Handle the click on the Start button
        System.out.println("Start button clicked");
    }

    private static void handleNextButtonClick() {
        // Handle the click on the Next button
        System.out.println("Next button clicked");
    }
}