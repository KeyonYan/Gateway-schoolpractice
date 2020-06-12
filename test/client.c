#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define IP "127.0.0.1"
#define PORT 11278

void main(int argc, char *argv[]) {
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(IP);  //具体的IP地址
    serv_addr.sin_port = htons(PORT);  //端口
    printf("connecting...\n");
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("connect ok\n");
    //读取服务器传回的数据
    char buffer[40];
    memset(buffer, 0, sizeof(buffer));
    //recv(sock, buffer, sizeof(buffer)-1, 0);
 
    //printf("from Server: %s\n", buffer);
    if (argc < 1) {
        printf("No argv\n");
        return;
    } else {
        printf("argv[1]: %s\n", argv[1]);
    }
    strcpy(buffer, argv[1]);
    write(sock, buffer, sizeof(buffer));
    
    //关闭套接字
    close(sock);
    printf("close\n");
}