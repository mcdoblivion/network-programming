// Dong Minh Cuong - 18020255

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  // basic system data types
#include <sys/socket.h> // basic socket definitions
#include <netinet/in.h> // sockaddr_in{} and other Internet defns
#include <arpa/inet.h>  // inet(3) functions
#include <unistd.h>
#include <sys/un.h>

#define SERVER_PORT 9000
int main(int argc, char **argv)
{
    char serverAddress[100];
    int serverPort;
    int connSock;
    struct sockaddr_in servAddr;
    char sentBuf[1000], recvBuf[1000];
    int sentDataLen, recvDataLen;

    if (argc >= 3)
    {
        strcpy(serverAddress, argv[1]);
        serverPort = atoi(argv[2]);
    }
    else
    {
        strcpy(serverAddress, "127.0.0.1");
        serverPort = SERVER_PORT;
    }

    connSock = socket(AF_INET, SOCK_STREAM, 0);
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
    printf("Connected to the echo server ...\n");
    while (1)
    {
        printf("Input a message to the echo server (QUIT for exit): ");
        int i = 0;
        char c = getchar();
        while (c != '\n')
        {
            sentBuf[i++] = c;
            c = getchar();
        }

        if (strcmp(sentBuf, "QUIT") == 0)
        {
            strcpy(sentBuf, "@close");
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
        printf("Server: %s\n", recvBuf);

        if (strcmp(recvBuf, "500 bye") == 0) {
            break;
        }
    }
    close(connSock);
    return 1;
}