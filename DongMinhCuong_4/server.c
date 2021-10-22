// Dong Minh Cuong - 18020255

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 9000
#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
    int listenPort = SERVER_PORT;
    int servSock, cliSock;
    struct sockaddr_in servSockAddr, cliAddr;
    int servSockLen, cliAddrLen;
    char recvBuf[BUFFER_SIZE], sentBuf[BUFFER_SIZE];
    int sentDataLen, recvDataLen;

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
        printf("Waiting for a client ... \n");
        cliSock = accept(servSock, (struct sockaddr *)&cliAddr,
                         (socklen_t *)&cliAddrLen);
        printf("Received a connection from a client %s\n", inet_ntoa(cliAddr.sin_addr));
        while (1)
        {
            recvDataLen = read(cliSock, recvBuf, BUFFER_SIZE - 1);
            recvBuf[recvDataLen] = '\0';
            if (recvDataLen == 0 || strcmp(recvBuf, "@close") == 0)
            {
                strcpy(sentBuf, "500 bye");
                write(cliSock, sentBuf, strlen(sentBuf));
                close(cliSock);
                printf("Close the connection\n");
                break;
            }

            printf("Client: %s\n", recvBuf);

            for (int i = 0; i < 10; i++) {}

            strcpy(sentBuf, "Error");

            if (strcmp(recvBuf, "HELLO Server") == 0)
            {
                strcpy(sentBuf, "200 HELLO Client");
            }

            if (strcmp(recvBuf, "USER NAME") == 0)
            {
                strcpy(sentBuf, "210 User name OK");
                write(cliSock, sentBuf, strlen(sentBuf));
            }

            if (strcmp(recvBuf, "USER AGE") == 0)
            { 
                strcpy(sentBuf, "210 User age OK");
            }

            write(cliSock, sentBuf, strlen(sentBuf));
            printf("Sent a message to the client: %s\n", sentBuf);
        }
    }
}