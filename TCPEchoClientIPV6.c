#include <stdio.h>   // For printf(), fprintf(), perror()
#include <sys/socket.h> // For socket(), connect(), send(), recv()
#include <arpa/inet.h>  // For sockaddr_in6, inet_pton()
#include <stdlib.h>  // For atoi()
#include <string.h>  // For exit(), memset()
#include <unistd.h>  // For close()

#define RECVBUFSIZE 32  // 受信バッファサイズ
#define DEFAULT_PORT 7  // echo サービスの well-known ポート

void errorHandler(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock;  // socket descriptor
    struct sockaddr_in6 echoServerAddr;  // IPv6 アドレス構造体
    unsigned short echoServerPort;
    char *serverIP;
    char *echoString;
    char echoBuffer[RECVBUFSIZE];
    unsigned int echoStringLen;
    int recvBytes, totalRecvBytes;

    // 引数のバリデーション
    if ((argc < 3) || (argc > 4)) {
        fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Server Port>]\n", argv[0]);
        exit(1);
    }

    serverIP = argv[1];
    echoString = argv[2];

    // ポートのバリデーション
    if (argc == 4) {
        echoServerPort = atoi(argv[3]);
    } else {
        echoServerPort = DEFAULT_PORT;
    }

    // ソケット作成 (IPv6)
    if ((sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        errorHandler("socket() failed");
    }

    // サーバーアドレス構造体の初期化
    memset(&echoServerAddr, 0, sizeof(echoServerAddr));  // echoServerAddr 構造体のゼロクリア
    echoServerAddr.sin6_family = AF_INET6;  // IPv6
    echoServerAddr.sin6_port = htons(echoServerPort);  // ポート番号の設定

    // IPv6 アドレスの変換
    if (inet_pton(AF_INET6, serverIP, &echoServerAddr.sin6_addr) <= 0) {
        errorHandler("inet_pton() failed");
    }

    // サーバーに接続
    if (connect(sock, (struct sockaddr *)&echoServerAddr, sizeof(echoServerAddr)) < 0) {
        errorHandler("connect() failed");
    }

    echoStringLen = strlen(echoString);

    // サーバーにメッセージを送信
    if (send(sock, echoString, echoStringLen, 0) != echoStringLen) {
        errorHandler("send() failed");
    }

    // サーバーからの応答を受信
    totalRecvBytes = 0;
    printf("Recieved: ");
    while (totalRecvBytes < echoStringLen) {
        if ((recvBytes = recv(sock, echoBuffer, RECVBUFSIZE - 1, 0)) <= 0) {
            errorHandler("recv() failed");
        }
        totalRecvBytes += recvBytes;
        echoBuffer[recvBytes] = '\0';
        printf("%s", echoBuffer);
    }
    printf("\n");

    // ソケットを閉じる
    close(sock);
    exit(0);
}
