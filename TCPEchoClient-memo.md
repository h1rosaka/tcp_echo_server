# ざっくり










```c
#define MAXPENDING 5 // 同時接続要求の最大数
#define RECVBUFSIZE 32 // 受信バッファサイズ
```

これはマクロ定義
マクロは、コンパイルする前に処理され、ある文字列を別の文字列に置き換える。
- http://www.cvl.cs.chubu.ac.jp/lecture/appc/2016.hidden/it2/09/
```
#define 置換前の文字列 置換後の文字列
```


```c
void errorHandler (char *msg) {
  perror(msg);
  exit(1);
}
```
エラー処理関数。
文字列リテラル(""で囲まれた文字)を受け取って、それをperrorで標準エラー出力に出す。
引数がポインタになってるのに、文字列リテラル渡せるのは、文字列リテラルが評価されたときに、先頭のアドレス(ポインタ)を返す仕組みになってるから。



perror()
```c
void perror(
   const char *message
);
```
>perror 関数は、エラー メッセージを stderr に印刷します。
- https://learn.microsoft.com/ja-jp/cpp/c-runtime-library/reference/perror-wperror?view=msvc-170
- 引数は、ポインタだが、


exit(1)
- プログラムを終了し、ホスト環境に制御を戻す。
- 引数にステータスを取れて、0は正常終了。それ以外は以上終了で、1を使うのが一般的。
- https://www.ibm.com/docs/ja/i/7.3?topic=functions-exit-end-program


atoi()
- 文字列を整数に変える。



```c
 // create socket
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    errorHandler("socket() failed");
  }
```
- 変数sockにファイルディスクリプタを格納
- socket関数の成功時には、ファイルディスクリプタが、失敗時には-1が返る
- PF_INETなどの定数ははsys/socket.hの中で定義されている。
- PF_INET
    - PF(プロトコルファミリー)(通信ドメイン)を指定
    - IPV4を意味する
    - IPV6の時は、PF_INET6
- SOCK_STREAM
    - ソケットの種類を指定
    - TCPを利用することを指定している
- IPPROTO_TCP
    - 使用するプロトコルを指定
    - TCPプロトコルを指定している



```c
    // create address struct of echo server
    memset(&echoServerAddr, 0, sizeof(echoServerAddr)); // echoServerAddr 構造体の領域をゼロクリアしておく
    echoServerAddr.sin_family = AF_INET;
    echoServerAddr.sin_addr.s_addr = inet_addr(serverIP);
    echoServerAddr.sin_port = htons(echoServerPort);
```

memset()
```c
void *memset(void *ptr, int value, size_t num);
```
メモリ領域を特定の値で初期化するための関数.

1.ptr:
    初期化したいメモリ領域の先頭アドレス。
2.value:
    メモリ領域に設定する値（unsigned char として解釈されます）。例えば、0 を指定するとゼロクリアされます。
3.num:
    初期化するバイト数。

inet()
```c
in_addr_t inet_addr(const char *cp);
```
文字列形式のIPv4アドレスを 32ビットのネットワークバイトオーダーの形式 に変換する関数
cp:
    IPv4アドレスを表す文字列ポインタ。
    例: "127.0.0.1" や "192.168.1.1"。
戻り値:
    成功: IPv4アドレスを in_addr_t 型（32ビット整数）で返す（ネットワークバイトオーダー）。
    失敗: INADDR_NONE を返す（0xFFFFFFFF）。


htons()
ホストバイトオーダーからネットワークバイトオーダーにポート番号を変換するための関数
ネットワークバイトオーダー (Network Byte Order) とは、ネットワークでデータを送受信する際に使用する特定のバイト順序のことです。これは ビッグエンディアン (Big-Endian) と呼ばれる形式で、最も重要なバイト（上位バイト）が最初に送信される順番です。
バイトオーダー（エンディアンネス）は、複数のバイトを使用して1つの値を表すときに、そのバイトがどの順番で格納されるかを指します。





```c
  // connect echo server
  if (connect(sock, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr)) < 0) {
    errorHandler("connect() failed");
  }
```
1. connect関数
構文:
```c
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
sockfd:
    ソケットのファイルディスクリプタ（この場合は変数 sock）。
    socket() 関数で作成されたソケットを指定します。
addr:
    接続先サーバのアドレス情報を持つ構造体へのポインタ。
    このコードでは &echoServerAddr（struct sockaddr_in型）を渡していますが、ポインタ型を一致させるために (struct sockaddr *) にキャストしています。
addrlen:
    addr に渡す構造体のサイズ（バイト数）。
    sizeof(echoServerAddr) で指定。

戻り値:
    成功時: 0
    失敗時: -1（エラーコードは errno に格納）
2. connectの役割
    クライアント側のソケットを**リモートサーバのアドレス（IPアドレスとポート番号）**に紐付けて接続を確立します。
    これが成功すると、クライアントとサーバの間でデータ送受信が可能になります。




```c
  // recieve message from echo server
  totalRecvBytes = 0;
  printf("Recieved: ");

  // 送信したデータの長さ(echoStringLen)にリーチするまでループ
  // 複数回に分けて送信されることがあるため。
  while (totalRecvBytes < echoStringLen) {
    if ((recvBytes = recv(sock, echoBuffer, RECVBUFSIZE-1, 0)) <= 0) {
      errorHandler("recv() failed");
    }
    totalRecvBytes += recvBytes;
    // 終端文字の追加
    echoBuffer[recvBytes] = '\0';
    printf("%s", echoBuffer);
  }
  printf("\n");
```


















# IPV6
主な変更点
### アドレス構造体を IPv6 用の sockaddr_in6 に変更

struct sockaddr_in6 echoServerAddr;

IPv6 アドレスを扱うため、IPv4 用の sockaddr_in から sockaddr_in6 に変更しました。

### inet_addr を inet_pton に変更 IPv6 アドレスをバイナリ形式に変換するため、inet_pton を使用します。

if (inet_pton(AF_INET6, serverIP, &echoServerAddr.sin6_addr) <= 0) {
    errorHandler("inet_pton() failed");
}
### ソケット作成時のアドレスファミリーを AF_INET6 に変更

if ((sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    errorHandler("socket() failed");
}
### IPv6 用のポート設定 IPv4 と同様ですが、sin6_port フィールドにポート番号を設定します。

echoServerAddr.sin6_port = htons(echoServerPort);