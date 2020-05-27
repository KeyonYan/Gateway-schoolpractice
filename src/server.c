/* Module: 网络通讯线程
 * Author: 闫科宇
 * Date:   2020-05-22 18:04:29
 * Desc:   处理客户端连接与接收命令数据，经校验符合通讯协议格式后，交给消息队列管理
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include "logger.h"
#include "server.h"


//const int BUF_SIZE = 4096;
const int PORT = 9090;
int serv_sock;

void onCreate();
void onListen();
void onAccept();
void onDestroy();

/**
 * BIO版本
 */
void *pthread_network() {
    logger("INFO", "[Net Module] >> \033[0;32m[...]");
    
    onCreate();
    logger("INFO", "[Net Module] >> \033[0;32m[Start]");
    while(1) {
        onAccept();
    }
    onDestroy();
    return NULL;
}

void onCreate() {
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(serv_sock >= 0);

    // 端口地址复用，解决程序中止而端口依旧占用问题
    int opt = 1;
    setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);
    
    int ret = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    assert(ret != -1);

    ret = listen(serv_sock, 20);
    assert(ret != -1);  
}

void onAccept() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &client_addr_size);
    assert(client_sock >= 0);
    
    char client_addr_str[50];
    sprintf(client_addr_str, "[Net Module] >> Client [%s:%d] Connection OK", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
    logger("INFO", client_addr_str);

    char buf[] = "Connection [Ok]";
    int size = write(client_sock, buf, sizeof(buf));
    assert(size != -1);

    close(client_sock);
}

void onDestroy() {
    close(serv_sock);
}
