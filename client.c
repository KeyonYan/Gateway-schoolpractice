#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void client_test() {
    //�����׽���
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    //����������ض���IP�Ͷ˿ڣ���������
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //ÿ���ֽڶ���0���
    serv_addr.sin_family = AF_INET;  //ʹ��IPv4��ַ
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
    serv_addr.sin_port = htons(6666);  //�˿�
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
 
    //��ȡ���������ص�����
    char buffer[40];
    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, sizeof(buffer)-1);
 
    printf("from Server: %s\n", buffer);
 
    //�ر��׽���
    close(sock);

    return 0;
}