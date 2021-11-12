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
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>
#define SERVER_PORT 1201
#define BUFFER_SIZE 1024

enum State
{
    WAITING,
    CONNECTED,
    DOWNLOAD,
};

long getFileSize(char fileName[]);
static void *doit(void *arg)
{
    int n, cliSock = *((int *)arg);
    char sentBuf[BUFFER_SIZE], recvBuf[BUFFER_SIZE];

    enum State state;
    state = WAITING;

    while (1)
    {
        n = read(cliSock, recvBuf, BUFFER_SIZE - 1);
        recvBuf[n] = '\0';
        printf("Client %d: %s\n", cliSock, recvBuf);
        if (strcmp(recvBuf, "QUIT") == 0)
        {
            state = WAITING;
            close(cliSock);
            printf("500 Bye\n");
            break;
        }

        if (state == WAITING)
        {
            if (strcmp(recvBuf, "HELLO Server") == 0)
            {
                state = CONNECTED;
                strcpy(sentBuf, "200 Hello Client");
            }
            else
            {
                state = WAITING;
                strcpy(sentBuf, "Invalid input");
            }
            write(cliSock, sentBuf, strlen(sentBuf));
        }
        else if (state == DOWNLOAD)
        {
            FILE *file = fopen(recvBuf, "rb");
            if (file != NULL)
            {
                strcpy(sentBuf, "220 File name=");
                strcat(sentBuf, recvBuf);
                strcat(sentBuf, "; File size=");
                unsigned long len = getFileSize(recvBuf);
                char lenBuf[BUFFER_SIZE];
                sprintf(lenBuf, "%lu", len);
                strcat(sentBuf, lenBuf);
                strcat(sentBuf, " byte(s); Start Downloading OK");
                write(cliSock, sentBuf, strlen(sentBuf));

                FILE *file = fopen(recvBuf, "rb");
                int sentSize = 0;
                while (sentSize < len)
                {
                    char fileBuf[BUFFER_SIZE];
                    int readSize = fread(fileBuf, 1, BUFFER_SIZE, file);
                    write(cliSock, fileBuf, readSize);
                    sentSize += readSize;
                }
            }
            else
            {
                strcpy(sentBuf, "410 File not found error");
                write(cliSock, sentBuf, strlen(sentBuf));
            }

            state = CONNECTED;
            fclose(file);
        }
        else if (state == CONNECTED)
        {
            if (strcmp(recvBuf, "DOWNLOAD") == 0)
            {
                state = DOWNLOAD;
                strcpy(sentBuf, "210 Download OK");
            }
            else
            {
                state = CONNECTED;
                strcpy(sentBuf, "Invalid input");
            }

            write(cliSock, sentBuf, strlen(sentBuf));
        }
        printf("Sent a message to the Client: %s\n", sentBuf);
    }

    free(arg);

    close(cliSock);
    return (NULL);
}

int main(int argc, char **argv)
{
    int listenPort = SERVER_PORT;
    int servSock;
    struct sockaddr_in servSockAddr, cliAddr;
    int servSockLen, cliAddrLen;

    pthread_t tid;
    int *iptr;

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
        printf("Waiting for a client ...\n");
        iptr = malloc(sizeof(int));
        *iptr = accept(servSock, (struct sockaddr *)&cliAddr, (socklen_t *)&cliAddrLen);
        printf("Received a connection from a client %s\n", inet_ntoa(cliAddr.sin_addr));
        pthread_create(&tid, NULL, &doit, (void *)iptr);
    }
}

long getFileSize(char fileName[])
{
    FILE *file = fopen(fileName, "r");
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fclose(file);

    return len;
}