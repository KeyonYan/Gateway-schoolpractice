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
#include <assert.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include "logger.h"
#include "server.h"

#define BUFF_SIZE 1024
#define backlog 7
#define ser_port 11277
#define CLI_NUM 3

int client_fds[CLI_NUM]; // all client fd
struct sockaddr_in client_addr_list[CLI_NUM]; // all client addr
char logBuf[256];
 
void *pthread_network()
{
    int listen_socket;
    int i;  
    char input_message[BUFF_SIZE];
    char resv_message[BUFF_SIZE];
 
    struct sockaddr_in ser_addr;
    ser_addr.sin_family= AF_INET;    //IPV4
    ser_addr.sin_port = htons(ser_port);
    ser_addr.sin_addr.s_addr = INADDR_ANY;  //指定的是所有地址
 
    // socket
    int ret;
    ret = listen_socket = socket(AF_INET,SOCK_STREAM,0);
    assert(ret >= 0);
    
    int opt = 1;
    setsockopt(listen_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)); 
 
    // bind
    ret = bind(listen_socket, (const struct sockaddr *)&ser_addr,sizeof(ser_addr));
    assert(ret >= 0);
    
    // listen
    ret = listen(listen_socket, backlog);
    assert(ret >= 0);
 
    // fd_set
    fd_set ser_fdset;
    int max_fd=1;
    struct timeval mytime;
    logger("INFO", "[Net Module] Wait for connnect.");
 
    while(1)
    {
        mytime.tv_sec=27;
        mytime.tv_usec=0;
        // clear fd_set
        FD_ZERO(&ser_fdset);
 
        // add standard input
        FD_SET(0,&ser_fdset);
        if(max_fd < 0)
        {
            max_fd = 0;
        }
 
        //add listen socket to fd_set
        FD_SET(listen_socket, &ser_fdset);
        if(max_fd < listen_socket)
        {
            max_fd = listen_socket;
        }
 
        //add client socket to fd_set
        for(i = 0; i < CLI_NUM; i++)
        {
            if(client_fds[i] != 0)
            {
                FD_SET(client_fds[i],&ser_fdset);
                if(max_fd < client_fds[i])
                {
                    max_fd = client_fds[i];
                }
            }
        }
 
        // select多路复用
        int ret = select(max_fd + 1, &ser_fdset, NULL, NULL, &mytime);
        if(ret < 0)
        {   
            perror("select failure\n");
            continue;
        } else if(ret == 0) {
            logger("INFO", "[Net Module] select time out!\n");
            continue;
        } else {
            //标准输入是否存在于ser_fdset集合中（也就是说，检测到输入时，做如下事情）
            if(FD_ISSET(0,&ser_fdset)) {
                printf("send message to: ");
                bzero(input_message,BUFF_SIZE);
                fgets(input_message,BUFF_SIZE,stdin);
 
                for(i = 0; i < CLI_NUM; i++)
                {
                    if(client_fds[i] != 0)
                    {
                        printf("client_fds[%d]=%d\n", i, client_fds[i]);
                        send(client_fds[i], input_message, BUFF_SIZE, 0);
                    }
                }
            }
            // if listen_socket can read/write (client is connecting)
            if(FD_ISSET(listen_socket, &ser_fdset)) {
                struct sockaddr_in client_address;
                socklen_t address_len;
                int client_sock_fd = accept(listen_socket, (struct sockaddr *)&client_address, &address_len);
                if(client_sock_fd > 0) {
                    int flags = -1;
                    //一个客户端到来分配一个fd，CLI_NUM=3，则最多只能有三个客户端，超过4以后跳出for循环，flags重新被赋值为-1
                    for(i = 0; i < CLI_NUM; i++) {
                        if(client_fds[i] == 0) {
                            flags = i;
                            client_fds[i] = client_sock_fd;
                            client_addr_list[i] = client_address;
                            break;
                        }
                    }
 
                    if (flags >= 0) {
                        sprintf(logBuf, "[Net Module] New Client[%d][%s:%d] connect successfully\n", flags, inet_ntoa(client_addr_list[flags].sin_addr), client_addr_list[flags].sin_port);
                        logger("INFO", logBuf);
                    } else { // flag = -1
                        bzero(input_message,BUFF_SIZE);
                        sprintf(input_message, "the client num is full! can't join!\n");
                        send(client_sock_fd, input_message, BUFF_SIZE, 0);
                        sprintf(logBuf, "[Net Module] New Client try connect, but max-num is full");
                        logger("WARNNING", logBuf);
                    }
                }   
            }
 
        }
 
        //deal with the message
        for(i = 0; i < CLI_NUM; i++) {
            if(client_fds[i] != 0) {
                // if client socket have data (receive data from client)
                if(FD_ISSET(client_fds[i], &ser_fdset)) {
                    bzero(resv_message, BUFF_SIZE);
                    int byte_num = read(client_fds[i], resv_message, BUFF_SIZE);
                    if(byte_num > 0) {
                        sprintf(logBuf, "[Net Module] Message from client[%d][%s:%d]: %s\n", i, inet_ntoa(client_addr_list[i].sin_addr), client_addr_list[i].sin_port, resv_message);
                    } else if(byte_num < 0) {
                        sprintf(logBuf, "[Net Module] Read error from client[%d][%s:%d]", client_fds[i], inet_ntoa(client_addr_list[i].sin_addr), client_addr_list[i].sin_port);
                        logger("ERROR", logBuf);
                    } else { // client disconnect, cancel it fdset and set fd=0
                        sprintf(logBuf, "[Net Module] Client[%d][%s:%d] exit!\n", i, inet_ntoa(client_addr_list[i].sin_addr), client_addr_list[i].sin_port);
                        logger("INFO", logBuf);
                        FD_CLR(client_fds[i], &ser_fdset);
                        client_fds[i] = 0;
                        continue;  // 这里如果用break的话一个客户端退出会造成服务器也退出。 
                    }
                }
            }
        }   
    }
    return NULL;
}