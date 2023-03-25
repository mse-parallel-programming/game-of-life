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

/**
 * @author Kacper Urbaniec
 * @version 2023-03-24
 */
public class Main {
    public static void main(String[] args) {

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

        var objectMapper = new ObjectMapper();
        var startMsgObj = new StartMessage();
        var input = new GameInput();
        input.size = size;
        input.grid = grid;
        // var benchmarkInput = new BenchmarkInput();
        // benchmarkInput.generations = 1000;
        // benchmarkInput.iterations = 3;
        // benchmarkInput.dynamic = true;
        // benchmarkInput.threadCount = 12;
        startMsgObj.input = input;
        // startMsgObj.benchmarkInput = benchmarkInput;

        String startMsg = "";
        try {
            startMsg = objectMapper.writeValueAsString(startMsgObj);
            System.out.println(startMsg);
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        String hostname = "localhost";
        int portNumber = 1234;
        try (
                var socket = new Socket(hostname, portNumber);
                var out = new PrintWriter(socket.getOutputStream(), true);
                var in = new BufferedReader(new InputStreamReader(socket.getInputStream()))
        ) {
            var initialMsg = in.readLine();
            System.out.println(initialMsg);
            out.println(startMsg);

            // TODO: exit? error handling?
            for(var i = 0; i < 10; ++i) {
                var msg = in.readLine();
                // TODO: Receive json diff
                // msg = msg.replaceAll(";", "\n");
                var msgObj = objectMapper.readValue(msg, UpdateMessage.class);
                System.out.println(msgObj.toString());
                Thread.sleep(1500);
                if ((i+1) == 10)
                    out.println("end!");
                else
                    out.println("next!");
            }

        } catch (Exception ex) {
            ex.printStackTrace();
        }

        System.out.println("Hello world!");
    }
}