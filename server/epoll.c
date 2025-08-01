#include "head.h"

// 向epoll对象里添加要监听的文件
int add_epoll(int epoll_fd, int fd){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
}