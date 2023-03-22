import java.io.DataInputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;

public class Server {

    public ServerSocket serverSocket;
    private Socket clientSocket;
    private DataInputStream inputStream;
    void startServerAndAccept() {
        int portNumber = 3000;
        try {
            serverSocket = new ServerSocket(portNumber);
            System.out.println("Server started and listening on port " + portNumber);
            clientSocket = serverSocket.accept();
            System.out.println("Java server connected to client " + clientSocket.getInetAddress());
            inputStream = new DataInputStream(clientSocket.getInputStream());
        } catch (IOException e) {
            System.err.println("Server could not start on port " + portNumber);
            e.printStackTrace();
        }
    }

    void stopServer() {
        System.out.println("Shutting down server & client ");
        try {
            if (serverSocket != null && !serverSocket.isClosed()) {
                serverSocket.close();
            }
        } catch (IOException e) {
            System.err.println("Couldn't shutdown server " + e);
            e.printStackTrace();
        }
        try {
            if(clientSocket != null && !clientSocket.isClosed()) {
                clientSocket.close();
            }
        } catch (IOException e) {
            System.err.println("Couldn't shutdown client " + e);
            e.printStackTrace();
        }
    }

    int[][] readValue() {
        if(inputStream == null) return null;

        int waited = 0;
        try {
            int possibleAmount = 0;
            while(possibleAmount == 0) {
             possibleAmount = inputStream.available();
             waited+= 100;
             Thread.sleep(100);
             if(waited >= 10*1000) {
                 System.out.println("No message since 10 seconds disconnected");
                 stopServer();
                 return null;
             }
            }
            byte[] bytes;
            bytes = inputStream.readNBytes(possibleAmount);

            String str = new String(bytes);
            System.out.println(bytes.length);
            if(str.contains("exit")) {
                System.out.println("Java server received exit command");
                stopServer();
            } else {// 4 bytes per integer

// Reconstruct the 2D integer array from the byte array
                int[][] arr = new int[MainUI.SQUARE_ROW][MainUI.SQUARE_COLUMN];
                for (int i = 0; i < MainUI.SQUARE_ROW; i++) {
                    for (int j = 0; j < MainUI.SQUARE_COLUMN; j++) {
                        int index = i * MainUI.SQUARE_COLUMN + j;
                        arr[i][j] = bytes[index];
                    }
                }
                return arr;
            }
            System.out.println(str);
//            ByteBuffer byteBuffer = ByteBuffer.wrap(bytes);
//            IntBuffer intBuffer = byteBuffer.asIntBuffer();
//            int[][] intArray = new int[intBuffer.remaining() / Chessboard.SQUARE_COLUMN][Chessboard.SQUARE_ROW];
//            intBuffer.get(intArray[0]);
//
//            if (inputObject instanceof int[][]) {
//                int[][] intArray = (int[][]) inputObject;
//                System.out.println("Java server received 2D int array:");
//                return intArray;
//            } else if ("exit".equals(inputObject)) {
//                System.out.println("Java server received exit command");
//                stopServer();
//            }
        } catch (IOException | InterruptedException e) {
            System.err.println(e.getMessage());
            e.printStackTrace();
        }
        return null;
    }
}
