#include "head.h"

// 子进程的工作函数
int son_main(int local_socket){
    for (;;) {
        // 1. 读任务：读取local_socket,等待父进程通信
        int net_fd;
        printf("son:son_main:ready to get new_fd\n");
        local_recv(local_socket, &net_fd); // 这里没有阻塞住
        printf("son:son_main:get net_fd = %d, go to work!\n", net_fd);
        // 2. 工作：跟客户端通信
        do_work(net_fd);
        // 3. 下班：通知服务器服务完毕
        printf("work finish!\n");
        close(net_fd); // BUG:没有close,会一直循环。
        send(local_socket, "done", 4, 0);
    }

    return 0;
}