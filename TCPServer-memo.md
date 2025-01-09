

```c
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
    close(clientSock);  // クライアントとの接続を終了
}

```
recv()
- 第1引数にclientSock（クライアントとの通信ソケット）を指定。
- 第2引数にechoBufferを指定し、受信したデータをこのバッファに格納します。
- 第3引数にRECVBUFSIZE（最大受信サイズ）を指定。
- 第4引数に0を指定して、標準的な受信モードで通信します。
- 返り値
    - 正常に実行された場合:メッセージの長さ(バイト単位)
    - 正常に実行されなかった場合: -1




















## 
IPV6
変更点
1. ソケット作成で AF_INET を AF_INET6 に変更
IPv6 を使用する場合、アドレスファミリーを AF_INET6 に設定します。

2. struct sockaddr_in を struct sockaddr_in6 に変更
IPv6 用のアドレス構造体である sockaddr_in6 を使用します。

3. アドレス指定の関数を IPv6 用に変更
inet_pton() や IN6ADDR_ANY_INIT などを使用して IPv6 アドレスを扱います。

