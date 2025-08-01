#include "head.h"

// 使用epoll 代替 select 进行IO多路复用
#define MAX_EVENTS 10
#define BUF_SIZE 128

int main(){
    // 创建socket
    char* ip = "192.168.220.131";
    char* port = "8080";
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip);
    sockaddr.sin_port = htons(atoi(port));

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socket_fd, -1, "socket");

    // 连接
    int connect_no = connect(socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    ERROR_CHECK(connect_no, -1, "connect");

    char buf[128] = {0};
    int ret = recv(socket_fd, buf, sizeof(buf),0);
    ERROR_CHECK(ret, -1, "recv");
    printf("buf:%s",buf);


    close(socket_fd);

    return 0;
}