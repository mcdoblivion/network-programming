// Dong Minh Cuong - 18020255
// Thuc hanh tuan 8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdbool.h>

#define SERVER_PORT 1201
#define BUFFER_SIZE 1024

bool isFileInfoMsg(char msg[]);

unsigned long getFileSize(char msg[]);

char *getFileName(char msg[]);

int main(int argc, char **argv)
{
    char serverAddress[100];
    int serverPort;
    int connSock;
    struct sockaddr_in servAddr;
    char sentBuf[BUFFER_SIZE], recvBuf[BUFFER_SIZE];
    int sentDataLen, recvDataLen;

    if (argc >= 2)
    {
        strcpy(serverAddress, argv[1]);
    }
    else
    {
        strcpy(serverAddress, "127.0.0.1");
    }

    connSock = socket(AF_INET, SOCK_STREAM, 0);
    serverPort = SERVER_PORT;
    if (connSock < 0)
    {
        printf("Error when creating socket\n");
        exit(1);
    }

    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(serverAddress);
    servAddr.sin_port = htons(serverPort);
    if (connect(connSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("Error when connecting!");
        exit(1);
    }

    printf("Connected to the Server ...\n");
    while (1)
    {
        printf("Input a message to the echo server (QUIT for exit): ");
        gets(sentBuf);

        if (strcmp(sentBuf, "QUIT") == 0)
        {
            write(connSock, sentBuf, strlen(sentBuf));
            break;
        }

        sentDataLen = write(connSock, sentBuf, strlen(sentBuf));
        if (sentDataLen < 0)
        {
            printf("Error when sending data\n");
            break;
        }

        recvDataLen = read(connSock, recvBuf, sizeof(recvBuf));

        if (recvDataLen < 0)
        {
            printf("Error when receiving data\n");
            break;
        }

        recvBuf[recvDataLen] = '\0';
        printf("Message received from server: %s\n", recvBuf);

        if (isFileInfoMsg(recvBuf))
        {
            long fileSize = getFileSize(recvBuf);
            char *fileName = getFileName(recvBuf);

            int writeSize = 0;
            FILE *file = fopen(fileName, "wb");
            while (writeSize < fileSize)
            {
                int recvFileSize = read(connSock, recvBuf, sizeof(recvBuf));
                fwrite(recvBuf, 1, recvFileSize, file);
                writeSize += recvFileSize;
                printf("Downloading  %.2f%%\n", writeSize * 100.f / fileSize);
            }
            fclose(file);
        }
    }

    close(connSock);
    return 1;
}

bool isFileInfoMsg(char msg[])
{
    return msg[0] == '2' && msg[1] == '2' && msg[2] == '0';
}

unsigned long getFileSize(char msg[])
{
    int i = 0;
    char *p = strtok(msg, ";");
    char *array[3];

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok(NULL, ";");
    }

    int len = strlen(array[1]);
    int start = strlen(" File size=");
    int end = len - strlen(" byte(s)");
    int newLen = end - start;
    char buf[newLen];

    for (int i = start; i < end; i++)
    {
        buf[i - start] = array[1][i];
    }

    return atol(buf);
}

char *getFileName(char msg[])
{
    int i = 0;
    char *p = strtok(msg, ";");
    char *array[3];

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok(NULL, ";");
    }

    int len = strlen(array[0]);
    int start = strlen("220 File name=");
    int end = len;
    int newLen = end - start;
    char *buf = malloc(newLen);

    for (int i = start; i < end; i++)
    {
        buf[i - start] = array[0][i];
    }

    return buf;
}