// Dong Minh Cuong - 18020255

package socket;

import java.io.*;
import java.net.*;
import java.util.*;

public class client {

    public static void main(String[] args) {
        // Server address
        final String serverHost = "localhost";
        final int serverPort = 1201;

        Socket clientSocket = null;
        BufferedWriter outStream = null;
        BufferedReader inStream = null;

        try {
            // Create socket with server at port 1201
            clientSocket = new Socket(serverHost, serverPort);

            outStream = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()));
            inStream = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

            System.out.println("Connected to server: " + serverHost + ":" + serverPort);
        } catch (UnknownHostException e) {
            System.err.println("Wrong host address: " + serverHost + ":" + serverPort);
            return;
        } catch (IOException e) {
            System.err.println("I/O thread error with " + serverHost + ":" + serverPort);
            return;
        }

        try {
            // Read data from keyboard
            Scanner scanner = new Scanner(System.in);
            String inputGuide = "(Available commands: ['HELLO Server', 'USER INFO', 'QUIT'])\n";
            String message = "Enter something:";

            while (true) {
                System.out.println(inputGuide + message);
                String input = scanner.nextLine();

                outStream.write(input);
                outStream.newLine();
                outStream.flush();

                String response = inStream.readLine();
                System.out.println("Server: " + response);

                if (response.equals("210 OK")) {
                    message = "Enter user info in JSON format:";
                } else if (response.equals("400 User Info Error") || response.equals("400 Wrong JSON format")) {
                    message = "User info must be JSON format. Name contain only alphabet characters, age must be number!\nEnter something:";
                } else if (response.equals("400 Wrong Input")) {
                    message = "Wrong input! You need to follow the instructions!\nEnter something:";
                } else if (response.equals("500 bye")) {
                    System.out.println("Connection closed!");
                } else {
                    message = "Enter something:";
                }

                if (input.equals("QUIT"))
                    break;
            }

            outStream.close();
            inStream.close();
            clientSocket.close();
            scanner.close();
        } catch (UnknownHostException e) {
            System.err.println("Trying to connect to unknown host: " + e);
        } catch (IOException e) {
            System.err.println("IOException:  " + e);
        }
    }

}