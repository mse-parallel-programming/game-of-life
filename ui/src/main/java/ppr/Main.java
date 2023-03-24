package ppr;

import com.fasterxml.jackson.databind.ObjectMapper;
import ppr.messages.BenchmarkInput;
import ppr.messages.GameInput;
import ppr.messages.StartMessage;

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
        String hostname = "localhost";
        int portNumber = 1234;

        var objectMapper = new ObjectMapper();
        var startMsgObj = new StartMessage();
        var input = new GameInput();
        input.size = 3;
        input.grid = List.of(Arrays.asList(true, false, true));
        var benchmarkInput = new BenchmarkInput();
        benchmarkInput.generations = 1000;
        benchmarkInput.iterations = 3;
        benchmarkInput.dynamic = true;
        benchmarkInput.threadCount = 12;
        startMsgObj.input = input;
        startMsgObj.benchmarkInput = benchmarkInput;

        String startMsg = "";
        try {
            startMsg = objectMapper.writeValueAsString(startMsgObj);
            System.out.println(startMsg);
        } catch (Exception ex) {
            ex.printStackTrace();
        }


        try (
                var socket = new Socket(hostname, portNumber);
                var out = new PrintWriter(socket.getOutputStream(), true);
                var in = new BufferedReader(new InputStreamReader(socket.getInputStream()))
        ) {
            var msg = in.readLine();
            System.out.println(msg);
            out.println(startMsg);
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        System.out.println("Hello world!");
    }
}