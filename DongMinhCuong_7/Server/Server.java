// Dong Minh Cuong - 18020255

package Server;

import java.io.*;
import java.nio.file.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {
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

        private static void sendMessage(BufferedWriter outStream, String message) throws IOException {
            outStream.write(message);
            outStream.flush();
        }

        @Override
        public void run() {
            try {
                // Open I/O thread on Socket
                BufferedReader inStream = new BufferedReader(new InputStreamReader(socketOfServer.getInputStream()));
                BufferedWriter outStream = new BufferedWriter(new OutputStreamWriter(socketOfServer.getOutputStream()));
                FileInputStream file = null;

                Boolean readyToDownload = false, isDownloading = false;
                while (true) {
                    // Read data from client
                    char[] recvBuffer = new char[1024];
                    int len = inStream.read(recvBuffer, 0, recvBuffer.length);
                    String recvString = new String(recvBuffer, 0, len);
                    System.out.println("Client: " + recvString);

                    // Handle write data to client
                    if (recvString.equals("QUIT")) {
                        sendMessage(outStream, "500 Bye");
                        break;
                    } else if (recvString.equals("HELLO Server")) {
                        sendMessage(outStream, "200 Hello Client");
                    } else if (recvString.equals("DOWNLOAD")) {
                        readyToDownload = true;
                        isDownloading = false;
                        sendMessage(outStream, "200 Ready to download");
                    } else if (readyToDownload) {
                        String filePath = "Server/" + recvString;
                        File temp = new File(filePath);
                        if (!temp.exists()) {
                            sendMessage(outStream, "400 File does not exist");
                        } else {
                            long fileSize = Files.size(Paths.get(filePath));
                            isDownloading = true;
                            file = new FileInputStream(filePath);
                            sendMessage(outStream,
                                    "200 File exist; file name is " + recvString + " file size is " + fileSize
                                            + " bytes");
                        }
                        readyToDownload = false;
                    } else if (isDownloading) {
                        int c, count = 0;
                        char[] buffer = new char[1024];
                        while (count < buffer.length) {
                            c = file.read();
                            if (c == -1)
                                break;
                            buffer[count] = (char) c;
                            count++;
                        }
                        if (count > 0) {
                            sendMessage(outStream, new String(buffer, 0, count));
                        } else {
                            sendMessage(outStream, "200 File downloaded");
                            isDownloading = false;
                            readyToDownload = false;
                            file.close();
                        }
                    } else {
                        sendMessage(outStream, "400 Wrong Input");
                    }
                }

                if (inStream != null) {
                    inStream.close();
                }
                if (outStream != null) {
                    outStream.close();
                }
            } catch (
            IOException e) {
                System.out.println(e);
                e.printStackTrace();
            }
        }
    }
}