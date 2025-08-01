#include "head.h"


int main(){

    // 1.启动池中进程:创建子进程
    // 每一个子进程都要有一个单独的socket对象，需要记录子进程的信息
    son_t list[POOL_SIZE];
    memset(list, 0 ,sizeof(list));
    init_pool(list, POOL_SIZE);

    // 监听端口
    int socket_fd;
    init_socket(&socket_fd, "8080", "192.168.220.131");

    // IO多路复用：新连接进来、子进程发信息进来
    int epoll_fd = epoll_create1(0);
    add_epoll(epoll_fd, socket_fd);
    struct epoll_event ready_event[LISTEN_EVENTS];

    // 监听每一个local_socket,子进程发来的消息
    for (int i = 0; i < POOL_SIZE; i++){
        add_epoll(epoll_fd, list[i].local_socket);
    }
    
    for(;;){
        memset(ready_event, 0, sizeof(ready_event));
        int nfds = epoll_wait(epoll_fd, ready_event, LISTEN_EVENTS, -1);
        for (int i = 0; i < LISTEN_EVENTS; i++){
            int ready_fd = ready_event[i].data.fd;
            // 有新连接
            if (ready_fd == socket_fd){
                int new_fd = accept(socket_fd, NULL, NULL);
                printf("main:new_fd connect: %d\n",new_fd);
                // 选取空闲子进程
                to_son(list, POOL_SIZE, new_fd);
                printf("main:send new_fd to son\n");
                // 子进程接手了，父进程可以断开连接了
                close(new_fd);
            } else {
                // 子进程发信息过来，读取信息，并修改忙闲状态
                char buf[BUF_SIZE] = {0};
                int ret = recv(ready_fd, buf, BUF_SIZE, 0);
                printf("main:recv %s from ready_fd:%d\n",buf,ready_fd);
                for (int i = 0; i < POOL_SIZE; i++){
                    // 找到是哪个子进程发来的消息
                    if (ready_fd == list[i].local_socket){
                        printf("main:son %d finish job!\n",ready_fd);
                        list[i].status = FREE;
                        break;
                    }
                }
                
            }
        }
        
    }

    return 0;
}