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

            while (true) {
                System.out.println("Enter something:");
                String input = scanner.nextLine();

                outStream.write(input);
                outStream.newLine();
                outStream.flush();
                System.out.println("Server: " + inStream.readLine());

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