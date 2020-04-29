#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

//const static int BUF_SIZE = 4096;
const static int port = 6666;

/**
 * ÍøÂçÍ¨Ñ¶Ä£¿é
 */
void pthread_network () {
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(serv_sock, 20);
    
    struct sockaddr_in client_addr;
    socklen_t clint_addr_size = sizeof(client_addr);
    int client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &clint_addr_size);

    char buf[] = "Connection [Ok]";
    write(client_sock, buf, sizeof(buf));

    close(client_sock);
    close(serv_sock);

}