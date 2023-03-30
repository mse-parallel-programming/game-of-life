import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.util.Random;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class MainUI extends JPanel {

    private static final boolean WITH_RANDOM_VALUES = false;
    private static final int SQUARE_SIZE = 30;
    public static final int SQUARE_ROW = 10;
    public static final int SQUARE_COLUMN = 10;
    private static final int BOARD_ROW_SIZE = SQUARE_SIZE * SQUARE_ROW;
    private static final int BOARD_COLUMN_SIZE = SQUARE_SIZE * SQUARE_COLUMN;
    private static JFrame mainFrame;
    private static int[][] input;

    public MainUI() {
        setPreferredSize(new Dimension(BOARD_ROW_SIZE, BOARD_COLUMN_SIZE));
    }

    private static void updateBoard(int[][] arr) {
        input = arr;
        mainFrame.repaint();
    }
    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        for (int row = 0; row < SQUARE_ROW; row++) {
            for (int col = 0; col < SQUARE_COLUMN; col++) {

                int x = col * SQUARE_SIZE;
                int y = row * SQUARE_SIZE;
                boolean isZero = (row + col) % 2 == 0;
                if(input != null) {
                    isZero = input[row][col] == 0;
                }
                if (isZero) {
                    g.setColor(Color.WHITE);
                } else {
                    g.setColor(Color.BLACK);
                }
                g.fillRect(x, y, SQUARE_SIZE, SQUARE_SIZE);
            }
        }
    }

    public static void main(String[] args) {
        mainFrame = new JFrame("Game of life UI");
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainFrame.setResizable(false);
        mainFrame.add(new MainUI());
        mainFrame.pack();
        mainFrame.setLocationRelativeTo(null);
        mainFrame.setVisible(true);

        Server server = new Server();

        while(true) {
            updateBoard(null);
            if(!WITH_RANDOM_VALUES) {
                server.startServerAndAccept();
            }
            try {
                while(WITH_RANDOM_VALUES || !server.serverSocket.isClosed()) {
                    int[][] intArray;
                    if(!WITH_RANDOM_VALUES) {
                        intArray = server.readValue();
                    } else {
                        intArray = generateRandomArray();
                    }
                    if(intArray == null) break;

                    updateBoard(intArray);
                    Thread.sleep(100);
                }
            } catch (InterruptedException e) {
                System.out.println("Interrupted exiting...");
                e.printStackTrace();
            }
        }
    }

    public static int[][] generateRandomArray() {
        int[][] arr = new int[SQUARE_ROW][SQUARE_COLUMN];
        Random rand = new Random();

        for (int i = 0; i < SQUARE_ROW; i++) {
            for (int j = 0; j < SQUARE_COLUMN; j++) {
                arr[i][j] = rand.nextInt(2);
            }
        }

        return arr;
    }
}
