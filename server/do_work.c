#include "head.h"

// 子进程工作
int do_work(int net_fd){

    // 测试
    send(net_fd, "hello", 5, 0);

    return 0;
}