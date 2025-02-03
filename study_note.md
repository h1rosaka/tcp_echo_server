
# https://reboooot.net/post/implement-tcp-echo-server/




# 実行
bash compile.sh 
./TCPEchoServer 5001
- 5000番は使われていたので、5001を使った。

# ターミナルでバツタブ開いて以下実行
tcp_echo_server $ lsof -i:5001
COMMAND     PID            USER   FD   TYPE             DEVICE SIZE/OFF NODE NAME
TCPEchoSe 15109 sakamotohiroaki    3u  IPv4 0x4e39c2b39db6a0f5      0t0  TCP *:commplex-link (LISTEN)


tcp_echo_server $ sudo lsof -p 15109

COMMAND     PID            USER   FD   TYPE             DEVICE SIZE/OFF     NODE NAME
TCPEchoSe 15109 sakamotohiroaki  cwd    DIR               1,16      320 10939249 /Users/sakamotohiroaki/src/tcp_echo_server
TCPEchoSe 15109 sakamotohiroaki  txt    REG               1,16    34256 10951560 /Users/sakamotohiroaki/src/tcp_echo_server/TCPEchoServer
TCPEchoSe 15109 sakamotohiroaki    0u   CHR              16,47   0t1168      889 /dev/ttys047
TCPEchoSe 15109 sakamotohiroaki    1u   CHR              16,47   0t1168      889 /dev/ttys047
TCPEchoSe 15109 sakamotohiroaki    2u   CHR              16,47   0t1168      889 /dev/ttys047
TCPEchoSe 15109 sakamotohiroaki    3u  IPv4 0x4e39c2b39db6a0f5      0t0      TCP *:commplex-link (LISTEN)


ソケットを識別するためのポイント:
ファイルディスクリプタの番号:

3u という部分はファイルディスクリプタ番号 3 のことを指しています。このファイルディスクリプタは「ユース（u）」として開かれており、データの読み書きが可能です。
IPv4 と TCP:

IPv4: このソケットが IPv4 プロトコルを使用していることを示します。ソケットがインターネット通信に使われる場合、通常このフィールドには IPv4 または IPv6 が表示されます。
TCP: 使用されているプロトコルが TCP であることを示します。TCP は、ソケット通信の中でよく使われる通信プロトコルです。
* と commplex-link:

*: ソケットが任意のネットワークインターフェースで接続を待機していることを示しています。つまり、ローカルコンピュータのすべてのIPアドレスで接続を受け入れる準備が整っているということです。
commplex-link: これは、ソケットがリッスンしているポートの名前を示しています。通常、サービス名やポート番号がここに表示されます。例えば、5000 番ポートに対応しているかもしれません。
(LISTEN):

最後に、(LISTEN) という状態が表示されています。これは、ソケットが接続を受け入れる準備ができており、待機している状態であることを示します。ネットワークプログラムが接続を受け付けるとき、LISTEN 状態に遷移します。




###
tcp_echo_server $ netstat -an | grep -i listen
tcp4       0      0  *.5001                 *.*                    LISTEN    

各部分の意味：
tcp4:

使用しているプロトコルが TCP であり、さらに IPv4 を使用していることを示しています。TCPは信頼性のある通信を提供するプロトコルで、IPv4はインターネットプロトコルのバージョン4です。
0 0:

最初の0は、受信キューのサイズ（待機中の受信データ量）を示します。
2番目の0は、送信キューのサイズ（待機中の送信データ量）を示します。
この場合、両方とも0なので、現在データの送受信が待機中でないことを意味します。
*.5001:

* は任意のネットワークインターフェースを意味し、5001 はリッスンしているポート番号です。つまり、サーバーが すべてのネットワークインターフェース の 5001番ポート で接続を待機していることを示しています。
*.*:

これは接続元のアドレスとポートを示しています。*.* は「どこからでも接続可能」という意味です。つまり、どんなIPアドレスとポートからでも、このサーバーの5001番ポートへの接続を受け付けているという状態です。
LISTEN:

このソケットが「接続待機中（リッスン状態）」であることを示します。つまり、サーバーが5001番ポートで受け入れる接続を待っている状態です。
要約：
この出力は、サーバーが 5001番ポート で すべてのネットワークインターフェース を通じて接続を待機している（リッスン状態にある）ことを示しています。接続待ちの状態であり、どんなIPアドレスからでも接続を受け付けています。


### 結果帰ってきた
tcp_echo_server $  ./TCPEchoClient 127.0.0.1 "test" 5001
Recieved: test

## 最初に起動した方のターミナルに、Handling...が新たに表示されている。
tcp_echo_server $ ./TCPEchoServer 5001
Handling client 127.0.0.1


# socket()





# いただいたコメントへの対応
- include を並べる順番のルールを持ちたい
    - アルファベット順に


- 32は少し小さいかも (ethernet のパケットサイズと比較すると、1 packet 処理するのに何回システムコールを呼ぶ必要がある? システムコールのオーバーヘッドはどのくらい?)
    - #define RECVBUFSIZE 32 // 受信バッファサイズ に対して
        - イーサーネットペイロード (= IPパケット)のサイズは46~1500バイト(p.81)
        -   


