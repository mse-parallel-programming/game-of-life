package ppr;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * @author Kacper Urbaniec
 * @version 2023-03-24
 */
public class Main {
    public static void main(String[] args) {
        String hostname = "localhost";
        int portNumber = 1234;

        try (
                var socket = new Socket(hostname, portNumber);
                var out = new PrintWriter(socket.getOutputStream(), true);
                var in = new BufferedReader(new InputStreamReader(socket.getInputStream()))
        ) {
            var msg = in.readLine();
            System.out.println(msg);
            out.println("Hey, Server!");
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        System.out.println("Hello world!");
    }
}