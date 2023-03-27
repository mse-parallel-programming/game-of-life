package ppr;

import com.fasterxml.jackson.databind.ObjectMapper;
import ppr.messages.*;
import ppr.util.Input;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-24
 */
public class Main {
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
            var size = 10;
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

            if (start.benchmarkInput == null)
                interactive(startJson, grid, 10, out, in);
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
}