package ppr;

import com.fasterxml.jackson.databind.ObjectMapper;
import ppr.messages.BenchmarkInput;
import ppr.messages.GameInput;
import ppr.messages.StartMessage;
import ppr.messages.UpdateMessage;
import ppr.util.Input;

import java.io.BufferedReader;
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
            // var benchmarkInput = new BenchmarkInput();
            // benchmarkInput.generations = 1000;
            // benchmarkInput.iterations = 3;
            // benchmarkInput.dynamic = true;
            // benchmarkInput.threadCount = 12;
            start.input = input;
            // startMsgObj.benchmarkInput = benchmarkInput;


            try {
                startJson = objectMapper.writeValueAsString(start);
                System.out.println(startJson);
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
            var initialMsg = in.readLine();
            System.out.println(initialMsg);
            out.println(startJson);

            // TODO: exit? error handling?
            for (var i = 0; i < 10; ++i) {
                // var test = in.readLine();
                // test = test.replaceAll(";", "\n");
                // System.out.println(test);

                var updateJson = in.readLine();
                var update = objectMapper.readValue(updateJson, UpdateMessage.class);
                // System.out.println(update.toString());
                update.updateGrid(grid);
                String gridString = grid.stream()
                        .map(n -> n
                                .stream()
                                .map(n2 -> {
                                    if (n2) return "1";
                                    else return "0";
                                })
                                .collect(Collectors.joining(" ", "", "\n"))
                        )
                        .collect(Collectors.joining(""));
                System.out.println(update.generation);
                System.out.println(gridString);

                Thread.sleep(1500);
                if ((i + 1) == 10)
                    out.println("end!");
                else
                    out.println("next!");
            }

        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}