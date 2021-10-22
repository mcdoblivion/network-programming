// Dong Minh Cuong - 18020255

package socket;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.regex.Pattern;

import org.json.simple.*;
import org.json.simple.parser.*;

public class server {

    public static boolean isAlphabet(String string) {
        return Pattern.matches("[a-zA-Z]+", string);
    }

    public static boolean isInteger(String string) {
        return Pattern.matches("[0-9]+", string);
    }

    public static void main(String args[]) throws IOException {

        ServerSocket listener = null;

        System.out.println("Waiting for connection from client...");
        int clientNumber = 0;

        // Create a ServerSocket at port 1201
        try {
            listener = new ServerSocket(1201);
        } catch (IOException e) {
            System.out.println(e);
            System.exit(1);
        }

        try {
            while (true) {
                // Accept a request from Client
                Socket socketOfServer = listener.accept();
                new ServiceThread(socketOfServer, clientNumber++).start();
            }
        } finally {
            listener.close();
        }

    }

    private static class ServiceThread extends Thread {

        private int clientNumber;
        private Socket socketOfServer;

        public ServiceThread(Socket socketOfServer, int clientNumber) {
            this.clientNumber = clientNumber;
            this.socketOfServer = socketOfServer;

            System.out.println("Received connection from client " + this.clientNumber + " at " + socketOfServer);
        }

        @Override
        public void run() {
            try {
                // Open I/O thread on Socket
                BufferedReader inStream = new BufferedReader(new InputStreamReader(socketOfServer.getInputStream()));
                BufferedWriter outStream = new BufferedWriter(new OutputStreamWriter(socketOfServer.getOutputStream()));
                int flag = 0;
                while (true) {
                    // Read data from client
                    String line = inStream.readLine();
                    System.out.println("Client: " + line);

                    // Handle write data to client
                    if (line.equals("QUIT")) {
                        outStream.write("500 bye");
                        outStream.newLine();
                        outStream.flush();
                        break;
                    } else if (line.equals("HELLO Server")) {
                        outStream.write("200 Hello Client");
                        outStream.newLine();
                        outStream.flush();
                    } else if (line.equals("USER INFO")) {
                        flag = 1;
                        outStream.write("210 OK");
                        outStream.newLine();
                        outStream.flush();
                    } else if (flag == 1) {
                        flag = 0;
                        try {
                            JSONObject jsonObject = (JSONObject) new JSONParser().parse(line);
                            String name = (String) jsonObject.get("User name");
                            String age = (String) jsonObject.get("User age");
                            if (isAlphabet(name) && isInteger(age)) {
                                outStream.write("211 User Info OK");
                                outStream.newLine();
                                outStream.flush();
                            } else {
                                outStream.write("400 User Info Error");
                                outStream.newLine();
                                outStream.flush();
                            }
                        } catch (Exception e) {
                            outStream.write("400 Wrong JSON format");
                            outStream.newLine();
                            outStream.flush();
                        }
                    } else {
                        outStream.write("400 Wrong Input");
                        outStream.newLine();
                        outStream.flush();
                    }
                }
            } catch (IOException e) {
                System.out.println(e);
                e.printStackTrace();
            }
        }
    }
}