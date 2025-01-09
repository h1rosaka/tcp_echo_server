#include <stdio.h> // For printf(), fprintf(), perror()
#include <sys/socket.h> // For socket(), bind(), connect(),recv(), send()
#include <arpa/inet.h> // For sockaddr_in, inet_ntoa()
#include <stdlib.h> // For atoi()
#include <string.h> // For exit(), memset()
#include <unistd.h> // For close()

#define MAXPENDING 5 // 同時接続要求の最大数
#define RECVBUFSIZE 32 // 受信バッファサイズ

void errorHandler (char *msg) {
  perror(msg);
  exit(1);
}

void HandleTCPClient(int clientSock) {
  char echoBuffer[RECVBUFSIZE];
  int recvMsgSize;
  
  // データの受信
  if ((recvMsgSize = recv(clientSock, echoBuffer, RECVBUFSIZE, 0)) < 0) {
    errorHandler("recv() failed");
  }

  while (recvMsgSize > 0) {
    // 受信したメッセージをそのまま返す
    if (send(clientSock, echoBuffer, recvMsgSize, 0) != recvMsgSize) {
      errorHandler("send() failed");
    }
    // 受信したデータを返した後、新しいデータを受信する.そしてそれを処理するべく、またループの先頭へ。
    if ((recvMsgSize = recv(clientSock, echoBuffer, RECVBUFSIZE, 0)) < 0) {
      errorHandler("recv() failed");
    }
  }
  close(clientSock);
}

int main (int argc, char *argv[]) {

  int serverSock;
  int clientSock;
  struct sockaddr_in echoServerAddr;
  struct sockaddr_in echoClientAddr;
  unsigned short echoServerPort;
  unsigned int clientLen;

  // arguments validation
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
    exit(1);
  }

  echoServerPort = atoi(argv[1]);

  // create socket
  if ((serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    errorHandler("socket() failed");
  }

  // create address struct of echo server
  memset(&echoServerAddr, 0, sizeof(echoServerAddr)); // echoServerAddr 構造体の領域をゼロクリアしておく
  echoServerAddr.sin_family = AF_INET;
  echoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY); // local ip 0.0.0.0
  echoServerAddr.sin_port = htons(echoServerPort);

  // bind socket　ソケットにポート番号などを割り当てる。(clientはconnect()が内部で適当に割り当てていた。)
  if (bind(serverSock, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr)) < 0) {
    errorHandler("bind() failed");
  }

  // listen port　　通信接続を待つための準備作業
  if (listen(serverSock, MAXPENDING) < 0) {
    errorHandler("listen() failed");
  }

  // accept connection-request from client
  // これは無限ループの書き方
  for (;;) {
    clientLen = sizeof(echoClientAddr);

    // acceptでクライアント側からの通信接続を待つ
    clientSock = accept(serverSock, (struct sockaddr *) &echoClientAddr, &clientLen);
    if (clientSock < 0) {
      errorHandler("accept() failed");
    }
    printf("Handling client %s\n", inet_ntoa(echoClientAddr.sin_addr));
    HandleTCPClient(clientSock);
  }

}