// Dong Minh Cuong - 18020255

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

using namespace std;

int main(int argc, char **argv)
{
    int listenPort = SERVER_PORT;
    int servSock, cliSock;
    struct sockaddr_in servSockAddr, cliAddr;
    int servSockLen, cliAddrLen;
    int n;

    if (argc >= 2)
    {
        listenPort = atoi(argv[1]);
    }
    servSock = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servSockAddr, sizeof(servSockAddr));
    servSockAddr.sin_family = AF_INET;
    servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servSockAddr.sin_port = htons(listenPort);

    servSockLen = sizeof(servSockAddr);
    if (bind(servSock, (struct sockaddr *)&servSockAddr, servSockLen) < 0)
    {
        perror("bind");
        exit(1);
    }
    if (listen(servSock, 10) < 0)
    {
        perror("listen");
        exit(1);
    }

    while (1)
    {
        cout << "Waiting for a client ..." << endl;
        cliSock = accept(servSock, (struct sockaddr *)&cliAddr,
                         (socklen_t *)&cliAddrLen);
        cout << "Received a connection from a client " << inet_ntoa(cliAddr.sin_addr) << endl;

        bool readyToDownload = false;
        bool isDownloading = false;
        bool isDownloaded = false;
        ifstream file;
        string fileName;

        while (1)
        {
            char recvBuf[BUFFER_SIZE], sentBuf[BUFFER_SIZE];

            n = read(cliSock, recvBuf, BUFFER_SIZE - 1);
            recvBuf[n] = '\0';
            string recvStr(recvBuf);

            if (n == 0 || recvStr == "@QUIT")
            {
                close(cliSock);
                cout << "Close the connection" << endl;
                break;
            }
            cout << "Received a message from the client: " << recvStr << endl;

            string sentStr;
            if (readyToDownload)
            {
                file = ifstream(recvStr, ios_base::binary);
                if (!file.good() && recvStr != "Downloading " + fileName)
                {
                    isDownloading = false;
                    sentStr = "400 File does not exist";
                }
                else
                {
                    fileName = recvStr;
                    isDownloading = true;
                    sentStr = "200 File exist; file name is " + fileName + "; file size is " + to_string(file.tellg()) + " bytes";
                }
                readyToDownload = false;
            }
            else if (isDownloading)
            {
                file.read(sentBuf, BUFFER_SIZE - 1);

                streamsize s = file ? BUFFER_SIZE - 1 : file.gcount();
                sentBuf[s] = '\0';
                sentStr = string(sentBuf);

                if (!file)
                {
                    file.close();
                    isDownloading = false;
                    isDownloaded = true;
                }
            }
            else if (isDownloaded)
            {
                isDownloaded = false;
                sentStr = "200 File downloaded";
            }
            else
            {
                if (recvStr == "HELLO Server")
                {
                    sentStr = "200 Hello Client";
                }
                else if (recvStr == "DOWNLOAD")
                {
                    readyToDownload = true;
                    sentStr = "200 Ready to download";
                }
                else
                {
                    sentStr = "Invalid input";
                }
            }

            strcpy(sentBuf, sentStr.c_str());
            write(cliSock, sentBuf, strlen(sentBuf));
            cout << "Sent a message to the client: " << sentStr << endl;
        }
    }
}