// Dong Minh Cuong - 18020255

package Client;

import java.io.*;
import java.nio.file.*;
import java.net.*;
import java.util.*;

public class Client {
    private static void sendMessage(BufferedWriter outStream, String message) throws IOException {
        outStream.write(message);
        outStream.flush();
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        // Server address
        System.out.println("Enter server IP address: ");
        final String serverHost = scanner.nextLine();
        System.out.println("Enter server port: ");
        final int serverPort = scanner.nextInt();
        scanner.nextLine();
        // final String serverHost = "localhost";
        // final int serverPort = 1201;

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
            scanner.close();
            return;
        } catch (IOException e) {
            System.err.println("I/O thread error with " + serverHost + ":" + serverPort);
            scanner.close();
            return;
        }

        try {
            // Read data from keyboard
            String inputGuide = "(Available commands: ['HELLO Server', 'DOWNLOAD', 'QUIT'])\n";
            String message = "Enter something:", input = "", fileName = "";
            FileOutputStream file = null;

            Boolean readyToDownload = false, isDownloading = false;
            while (true) {
                System.out.println(inputGuide + message);

                if (readyToDownload) {
                    input = scanner.nextLine();
                    fileName = input;
                } else if (isDownloading) {
                    input = "Downloading " + fileName;
                } else {
                    input = scanner.nextLine();
                }

                sendMessage(outStream, input);

                char[] recvBuffer = new char[1024];
                int len = inStream.read(recvBuffer, 0, recvBuffer.length);
                String recvString = new String(recvBuffer, 0, len);
                System.out.println("Server: " + recvString);

                if (input.equals("QUIT"))
                    break;

                if (recvString.equals("200 File downloaded")) {
                    isDownloading = false;
                    file.close();
                    System.out.println("File " + fileName + " downloaded!");
                    System.out.println("File size: " + Files.size(Paths.get("Client/" + fileName)) + " bytes");
                    message = "Enter something:";
                }

                if (isDownloading) {
                    for (int i = 0; i < recvString.length(); i++) {
                        file.write(recvString.charAt(i));
                    }
                    message = "Downloading " + fileName;
                } else if (recvString.equals("200 Ready to download")) {
                    readyToDownload = true;
                    message = "Enter file name to download:";
                } else if (recvString.startsWith("200 File exist")) {
                    isDownloading = true;
                    readyToDownload = false;
                    file = new FileOutputStream("Client/" + fileName);
                    message = "Downloading " + fileName;
                } else if (recvString.equals("400 File does not exist")) {
                    readyToDownload = false;
                    message = "File " + fileName + " does not exist!\nEnter something:";
                } else if (recvString.equals("400 Wrong Input")) {
                    message = "Wrong input! You need to follow the instructions!\nEnter something:";
                } else if (recvString.equals("500 Bye")) {
                    System.out.println("Connection closed!");
                } else {
                    message = "Enter something:";
                }
            }

            if (outStream != null) {
                outStream.close();
            }
            if (inStream != null) {
                inStream.close();
            }
            if (clientSocket != null) {
                clientSocket.close();
            }
            if (scanner != null) {
                scanner.close();
            }
        } catch (UnknownHostException e) {
            System.err.println("Trying to connect to unknown host: " + e);
        } catch (IOException e) {
            System.err.println("IOException:  " + e);
        }
    }
}