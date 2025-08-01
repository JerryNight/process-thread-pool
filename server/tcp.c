#include "head.h"


// 初始化socket对象
int init_socket(int * socket_fd, const char* port, const char* ip){
    *socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 设置reuse端口，不会因为timewait而阻塞
    int reuse = 1;
    setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip);
    sockaddr.sin_port = htons(atoi(port));

    bind(*socket_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));

    listen(*socket_fd, LISTEN_MAX);

    return 0;
}
