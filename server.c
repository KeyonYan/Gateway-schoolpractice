#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "logger.h"

//const int BUF_SIZE = 4096;
const int port = 6666;
int serv_sock;

/**
 * 网络通讯模块
 * 功能介绍：
 *  作为服务端，负责处理客户端的TCP连接
 *  接收的命令数据，经校验符合通讯协议格式后，交给消息队列管理
 */
void onCreate();
void onListen();
void onAccept();
void onDestroy();

/**
 * BIO版本
 */
void* pthread_network() {
    onCreate();
    while(1) {
        onListen();
        onAccept();
    }
    onDestroy();
}

void onCreate() {
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));        
}

void onListen() {
    listen(serv_sock, 20);
}

void onAccept() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &client_addr_size);

    char client_addr_str[50];
    sprintf(client_addr_str, ">> Client [%s:%d] Connection OK", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
    logger("INFO", client_addr_str);

    char buf[] = "Connection [Ok]";
    write(client_sock, buf, sizeof(buf));

    close(client_sock);
}

void onDestroy() {
    close(serv_sock);
}
