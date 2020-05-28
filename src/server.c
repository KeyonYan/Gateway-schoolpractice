/* Module: 网络通讯模块
 * Author: 闫科宇
 * Date:   2020-05-27 21:01:13
 * Desc:   提供TCP服务端通讯，支持多连接，采用单线程select实现I/O多路复用
 * 		   将客户端发送的命令数据转发至消息队列MQ中
 */

#include <stdio.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  
#include <string.h>  
#include <signal.h>
#include <assert.h>
#include <sys/msg.h>
#include "msgqueue.h"
#include "logger.h"
#define MAXLINE	    1024  // 缓冲区大小
#define LISTENLEN   10    // 侦听队列的长度
#define SERV_PORT   11278 // 监听端口号
#define FD_SETSIZE_ 1024  // FDSET集的大小

void *pthread_network()
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE_];
    struct sockaddr_in  client_addr_list[FD_SETSIZE_];
	ssize_t				n;
	fd_set				rset, allset;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
    char                logBuf[MAXLINE];
    MSG*                msg;
 
    logger("INFO", "[Net Module] >> \033[0;32m[...]");

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    int opt = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    assert(ret >= 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);
 
	ret = bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
    assert(ret >= 0);

	ret = listen(listenfd, LISTENLEN);
    assert(ret >= 0);

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE_; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

    logger("INFO", "[Net Module] >> \033[0;32m[Ok]");
    logger("INFO", "[Net Module] Wait for connnect.");
	for ( ; ; ) 
	{
		rset = allset;		/* structure assignment */
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
 
		if (FD_ISSET(listenfd, &rset)) /* new client connection */
		{
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clilen);

			for (i = 0; i < FD_SETSIZE_; i++) {
                if (client[i] < 0) {
					client[i] = connfd;	/* save descriptor */
                    client_addr_list[i] = cliaddr;
                    sprintf(logBuf, "[Net Module] New Client[%d][%s:%d] connect successfully\n", i, inet_ntoa(client_addr_list[i].sin_addr), client_addr_list[i].sin_port);
                    logger("INFO", logBuf);
					break;
				}
            }
			if (i == FD_SETSIZE_) {
				logger("WARNNING", "[Net Module] connections is full.");
                bzero(buf, sizeof(buf));
				sprintf(buf, "[FROM SERVER] Connections is full, please waiting.");
                write(connfd, buf, sizeof(buf));
                // exit(0);
                continue;
			}
 
			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */
 
			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}
 
		for (i = 0; i <= maxi; i++) 	/* check all clients for data */
		{	
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) 
			{
                bzero(buf, sizeof(buf));
				if ( (n = read(sockfd, buf, MAXLINE)) == 0) /* connection closed by client */ 
				{
					close(sockfd);
					FD_CLR(sockfd, &allset); // delete this fd
					client[i] = -1;
                    sprintf(logBuf, "[Net Module] Client[%d][%s:%d] exit!", i, inet_ntoa(client_addr_list[i].sin_addr), client_addr_list[i].sin_port);
                    logger("INFO", logBuf);
				} else {
                    sprintf(logBuf, "[Net Module] Message from client[%d][%s:%d]: %s", i, inet_ntoa(client_addr_list[i].sin_addr), client_addr_list[i].sin_port, buf);
                    logger("INFO", logBuf);
                    
                    // write to message queue
                    msg = msgCreate();
                    strcpy(msg->mtext, buf);
                    ret = mqWrite(msg);
                    if (ret == 0)
						logger("INFO", "[Net Module] Write a msg to MQ \033[0;32m[Ok]");
                    else
						logger("ERROR", "[Net Module] Write a msg to MQ [Fail]");
                    // echo to client
                    write(sockfd, buf, n); 
                    sprintf(logBuf, "[Net Module] Send to client[%d][%s:%d]: %s", i, inet_ntoa(client_addr_list[i].sin_addr), client_addr_list[i].sin_port, buf);
                }
				
				if (--nready <= 0)
					break;	/* no more readable descriptors */
			}
		}
	}
}
 