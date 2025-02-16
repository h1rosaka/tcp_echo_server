#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAXPENDING 5
#define RECVBUFSIZE 32

void errorHandler(const char *msg) {
    perror(msg);
    exit(1);
}

void HandleTCPClient(int clientSock) {
    char echoBuffer[RECVBUFSIZE];
    int recvMsgSize;

    if ((recvMsgSize = recv(clientSock, echoBuffer, RECVBUFSIZE, 0)) < 0) {
        errorHandler("recv() failed");
    }

    while (recvMsgSize > 0) {
        if (send(clientSock, echoBuffer, recvMsgSize, 0) != recvMsgSize) {
            errorHandler("send() failed");
        }
        if ((recvMsgSize = recv(clientSock, echoBuffer, RECVBUFSIZE, 0)) < 0) {
            errorHandler("recv() failed");
        }
    }

    close(clientSock);
}

int main(int argc, char *argv[]) {
    int serverSock;
    int clientSock;
    struct sockaddr_in6 echoServerAddr;  // IPv6 アドレス構造体
    struct sockaddr_in6 echoClientAddr;
    unsigned short echoServerPort;
    unsigned int clientLen;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }

    echoServerPort = atoi(argv[1]);

    // ソケット作成 (IPv6, TCP)
    if ((serverSock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        errorHandler("socket() failed");
    }

    // サーバーアドレス構造体の設定
    memset(&echoServerAddr, 0, sizeof(echoServerAddr));
    echoServerAddr.sin6_family = AF_INET6;  // IPv6
    echoServerAddr.sin6_addr = in6addr_any;  // 任意のローカル IPv6 アドレス
    echoServerAddr.sin6_port = htons(echoServerPort);

    // ソケットをバインド
    if (bind(serverSock, (struct sockaddr *)&echoServerAddr, sizeof(echoServerAddr)) < 0) {
        errorHandler("bind() failed");
    }

    // 接続待機
    if (listen(serverSock, MAXPENDING) < 0) {
        errorHandler("listen() failed");
    }

    // クライアント接続を待機
    for (;;) {
        clientLen = sizeof(echoClientAddr);
        clientSock = accept(serverSock, (struct sockaddr *)&echoClientAddr, &clientLen);
        if (clientSock < 0) {
            errorHandler("accept() failed");
        }

        // クライアント情報を表示
        char clientAddress[INET6_ADDRSTRLEN];
        if (inet_ntop(AF_INET6, &echoClientAddr.sin6_addr, clientAddress, sizeof(clientAddress)) != NULL) {
            printf("Handling client %s\n", clientAddress);
        } else {
            printf("Unable to get client address\n");
        }

        HandleTCPClient(clientSock);
    }

    close(serverSock);
    return 0;
}
