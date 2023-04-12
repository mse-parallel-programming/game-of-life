package ppr;

import com.fasterxml.jackson.databind.ObjectMapper;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.geometry.HPos;
import javafx.geometry.Pos;
import javafx.geometry.VPos;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Control;
import javafx.scene.control.Label;
import javafx.scene.layout.*;
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
public class Main extends Application {
    private static final boolean benchmarkMode = false;
    private static int size = 30;
    private static final int width = 600;
    private static final int height = 600;
    private static int generationCount = 0;
    private static final GridPane grid = new GridPane();
    private static Node[][] gridPaneArray = null;
    private static final Label generationLabel = new Label();
    private static PrintWriter out;
    private static StartMessage start;
    private static Button startButton;

    private ChangeListener<? super Number> widthChangeListener;
    private ChangeListener<? super Number> heightChangeListener;

    public static void main(String[] args) {

        {
            var textInput = "..............................".repeat(10) +
                    ".............................." +
                    ".............**..............." +
                    "..............*..............." +
                    ".............................." +
                    ".............................." +
                    ".............**..............." +
                    "............**................" +
                    "...............*.............." +
                    "..............*..............." +
                    ".............................." +
                    "..............................".repeat(10);

            var grid = Input.textInput(textInput);
            size = grid.size();


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
            if (benchmarkMode) {
                var benchmarkInput = new BenchmarkInput();
                benchmarkInput.generations = 10;
                benchmarkInput.iterations = 10;
                start.benchmarkInput = benchmarkInput;
            }

            // =================
            // (Optional) Customize thread config for server
            // =================
            // var threadConfig = new ThreadConfig();
            // threadConfig.dynamic = false;
            // threadConfig.threadCount = 1;
            // start.threadConfig = threadConfig;
        }


        String hostname = "localhost";
        int portNumber = 1234;
        try (
                var socket = new Socket(hostname, portNumber);
                var out = new PrintWriter(socket.getOutputStream(), true);
                var in = new BufferedReader(new InputStreamReader(socket.getInputStream()))
        ) {
            Main.out = out;
            Thread launchThread = new Thread(() -> launch(args));
            launchThread.start();
            if (!benchmarkMode) {
                interactive(in);
            } else {
                benchmark(in);
            }

        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private static String getStartJson() {
        var objectMapper = new ObjectMapper();
        try {
            String startJson = objectMapper.writeValueAsString(start);
            System.out.println(startJson);
            System.out.println();
            return startJson;
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return "";
    }

    public static void interactive(
            BufferedReader in
    ) throws IOException, InterruptedException {
        int generations = start.input.generations;
        var objectMapper = new ObjectMapper();
        var grid = start.input.grid;
        start.input.waitUntilManualOrNextIsTrue();
        out.println(getStartJson());

        for (var i = 0; i < generations; ++i) {
            start.input.waitUntilManualOrNextIsTrue();
            if (start.input.getEndProgram()) {
                break;
            }
            // Get diff
            var updateJson = in.readLine();
            var update = objectMapper.readValue(updateJson, UpdateMessage.class);

            update.updateGrid(grid);
            // Visualize grid
            String gridString = gridString(grid);
            updateChessboard(grid);
            Platform.runLater(() -> {
                generationCount = update.generation;
                if (!benchmarkMode) {
                    generationLabel.setText("\tGeneration: " + update.generation);
                }
            });
            System.out.println(update.generation);
            System.out.println(gridString);

            // Wait for a bit
            Thread.sleep(500);

            // Send if processing should continue or not
            boolean next = (i + 1) != generations;

            if (start.input.getNext()) {
                next = true;
                start.input.setNext(false);
            }

            var ackMessage = new UpdateAckMessage(next);
            var ackMessageJson = objectMapper.writeValueAsString(ackMessage);
            out.println(ackMessageJson);
        }
    }

    public static void benchmark(
            BufferedReader in
    ) throws IOException, InterruptedException {
        generationLabel.setText("Generations: " + start.benchmarkInput.generations + "\n Iterations: " + start.benchmarkInput.iterations);
        var objectMapper = new ObjectMapper();

        // Prohibit handleChessboardClick
        generationCount = 999;

        start.input.waitUntilManualOrNextIsTrue();
        out.println(getStartJson());

        var resultJson = in.readLine();
        var result = objectMapper.readValue(resultJson, BenchmarkMessage.class);

        System.out.println("Average time: " + String.format("%.4f", result.averageTime) + " ms");
        System.out.println("Min time: " + String.format("%.4f", result.minTime) + " ms");
        System.out.println("Max time: " + String.format("%.4f", result.maxTime) + " ms");

        Platform.runLater(() ->
                generationLabel.setText(
                        "Average time: " + String.format("%.4f", result.averageTime) + " ms\n" +
                                "Min time: " + String.format("%.4f", result.minTime) + " ms\n" +
                                "Max time: " + String.format("%.4f", result.maxTime) + " ms")
        );
        updateChessboard(result.grid);
        String gridString = gridString(result.grid);
        System.out.println(gridString);
    }

    @Override
    public void start(Stage primaryStage) {
        // Create the chessboard
        createChessboard(start.input.grid);

        // Create the Start and Next buttons
        startButton = new Button("Start");
        Button nextButton = new Button("Next");
        startButton.setOnAction(e -> handleStartButtonClick());
        nextButton.setOnAction(e -> handleNextButtonClick());

        // Create the layout
        BorderPane layout = new BorderPane();
        layout.setCenter(grid);

        var bottomMenu = new HBox();
        bottomMenu.setAlignment(Pos.CENTER_LEFT);
        var menuChildren = bottomMenu.getChildren();
        menuChildren.add(startButton);
        if (!benchmarkMode) {
            menuChildren.add(nextButton);
        }
        menuChildren.add(generationLabel);
        layout.setBottom(bottomMenu);

        // Keep aspect ratio
        // https://stackoverflow.com/a/66733052
        widthChangeListener = (observable, oldValue, newValue) -> {
            primaryStage.heightProperty().removeListener(heightChangeListener);
            primaryStage.setHeight(newValue.doubleValue());
            primaryStage.heightProperty().addListener(heightChangeListener);
        };
        heightChangeListener = (observable, oldValue, newValue) -> {
            primaryStage.widthProperty().removeListener(widthChangeListener);
            primaryStage.setWidth(newValue.doubleValue());
            primaryStage.widthProperty().addListener(widthChangeListener);
        };

        primaryStage.widthProperty().addListener(widthChangeListener);
        primaryStage.heightProperty().addListener(heightChangeListener);

        // Create the scene and show the stage
        Scene scene = new Scene(layout, width, height);
        primaryStage.setOnCloseRequest(event -> start.input.setEnd());
        primaryStage.setTitle("Game Of Life");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private static void createChessboard(List<List<Boolean>> input) {
        Platform.runLater(() -> {
            start.input.grid = input;
            grid.getChildren().clear();
        });

        boolean isEmpty = input == null || input.isEmpty();
        int rowSize = isEmpty ? size : input.size();
        gridPaneArray = new Node[rowSize][rowSize];

        for (int row = 0; row < rowSize; row++) {
            int colSize = isEmpty ? size : input.get(row).size();
            for (int col = 0; col < colSize; col++) {
                Boolean value = (row + col) % 2 == 0;
                if (!isEmpty) value = input.get(row).get(col);

                var rect = new StackPane();
                if (!value) rect.setStyle("-fx-background-color: white;");
                else rect.setStyle("-fx-background-color: black;");

                final int x = col;
                final int y = row;
                rect.setOnMouseClicked(e -> handleChessboardClick(x, y));
                Platform.runLater(() -> {
                    grid.add(rect, x, y);
                    gridPaneArray[y][x] = rect;
                });
            }
        }
        // Automatic resizing
        // https://stackoverflow.com/a/68055154
        for (int row = 0; row < rowSize; row++) {
            grid.getColumnConstraints().add(new ColumnConstraints(5, Control.USE_COMPUTED_SIZE, Double.POSITIVE_INFINITY, Priority.ALWAYS, HPos.CENTER, true));
            grid.getRowConstraints().add(new RowConstraints(5, Control.USE_COMPUTED_SIZE, Double.POSITIVE_INFINITY, Priority.ALWAYS, VPos.CENTER, true));
        }
    }

    private static void updateChessboard(List<List<Boolean>> input) {
        Platform.runLater(() -> {
            boolean isEmpty = input == null || input.isEmpty();
            int rowSize = isEmpty ? size : input.size();
            for (int row = 0; row < rowSize; row++) {
                int colSize = isEmpty ? size : input.get(row).size();
                for (int col = 0; col < colSize; col++) {
                    Boolean value = (row + col) % 2 == 0;
                    if (!isEmpty) value = input.get(row).get(col);
                    var rect = gridPaneArray[row][col];
                    if (!value) rect.setStyle("-fx-background-color: white;");
                    else rect.setStyle("-fx-background-color: black;");
                }
            }
        });
    }

    private static void handleChessboardClick(int x, int y) {
        if (generationCount > 0) {
            return;
        }
        // Handle the click on the chessboard
        var row = start.input.grid.get(y);
        var currentCellStatus = row.get(x);
        var newCellStatus = !currentCellStatus;
        // Update logic
        row.set(x, newCellStatus);
        // Update UI
        var rect = gridPaneArray[y][x];
        if (!newCellStatus) rect.setStyle("-fx-background-color: white;");
        else rect.setStyle("-fx-background-color: black;");
    }

    private static void handleStartButtonClick() {
        // Handle the click on the Start button
        System.out.println("Start button clicked");
        boolean newManual = !start.input.getManual();
        start.input.setManual(newManual);
        startButton.setText(newManual ? "Start" : "Pause");
        if (start.benchmarkInput != null) {
            generationLabel.setText("Starting benchmarking.");
            startButton.setVisible(false);
        }
    }

    private static void handleNextButtonClick() {
        // Handle the click on the Next button
        System.out.println("Next button clicked");
        start.input.setNext(true);
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
}