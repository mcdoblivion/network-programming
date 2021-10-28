// Dong Minh Cuong - 18020255

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#define BUFFER_SIZE 1024

using namespace std;

int main(int argc, char **argv)
{
    char serverAddress[100];
    int serverPort;
    int connSock;
    struct sockaddr_in servAddr;
    int sentDataLen, recvDataLen;

    cout << "Enter server address: ";
    cin >> serverAddress;
    cout << "Enter server port: ";
    cin >> serverPort;

    cout << "Server address is " << serverAddress << ":" << serverPort << endl;

    connSock = socket(AF_INET, SOCK_STREAM, 0);
    if (connSock < 0)
    {
        cout << "Error when creating socket" << endl;
        exit(1);
    }

    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(serverAddress);
    servAddr.sin_port = htons(serverPort);
    if (connect(connSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        cout << "Error when connecting!" << endl;
        exit(1);
    }
    cout << "Connected to the Server ..." << endl;
    cin.ignore();

    string message = "Enter something (@QUIT for exit, DOWNLOAD to download file): ";
    string input;
    string fileName;
    fstream file;
    bool isDownloading = false;
    bool readyToDownload = false;

    while (1)
    {
        char sentBuf[BUFFER_SIZE], recvBuf[BUFFER_SIZE];

        cout << message;

        if (readyToDownload)
        {
            getline(cin, input);
            fileName = input;
        }
        else if (isDownloading)
        {
            input = "Downloading " + fileName;
        }
        else
        {
            getline(cin, input);
        }

        if (input == "@QUIT")
        {
            write(connSock, sentBuf, strlen(sentBuf));
            break;
        }

        int lastPos = input.length() >= BUFFER_SIZE ? BUFFER_SIZE - 1 : input.length();
        strcpy(sentBuf, input.c_str());
        sentBuf[lastPos] = '\0';

        sentDataLen = write(connSock, sentBuf, strlen(sentBuf));
        if (sentDataLen < 0)
        {
            cout << "Error when sending data" << endl;
            break;
        }

        recvDataLen = read(connSock, recvBuf, sizeof(recvBuf) - 1);
        if (recvDataLen < 0)
        {
            cout << "Error when receiving data" << endl;
            break;
        }
        recvBuf[recvDataLen] = '\0';

        string recvStr(recvBuf);
        cout << "Message received from server: " << recvStr << endl;

        if (recvStr == "200 File downloaded")
        {
            isDownloading = false;
            cout << "Download " << fileName << " successfully!" << endl;
            cout << "File size: " << to_string(file.tellg()) << " bytes" << endl;
            file.close();
        }

        if (isDownloading)
        {
            if (!file.is_open())
            {
                file.open(fileName, ios::app);
            }
            file << recvBuf;
            message = "Downloading " + fileName + "\n";
        }
        else
        {
            if (recvStr == "200 Ready to download")
            {
                readyToDownload = true;
                message = "Enter file name to download: ";
            }
            else if (recvStr.rfind("200 File exist", 0) == 0 || recvStr == "Sending " + fileName)
            {
                isDownloading = true;
                readyToDownload = false;
                message = "Downloading " + fileName + "\n";
            }
            else
            {
                readyToDownload = false;
                message = "Enter something (@QUIT for exit, DOWNLOAD to download file): ";
            }
        }
    }
    close(connSock);
    return 1;
}