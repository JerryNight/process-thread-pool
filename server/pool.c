#include "head.h"

// 该文件处理缓冲池的逻辑
// 1. 创建子进程
// 2. 父子进程通过本地socket建立通信
// 3. 子进程阻塞，等待接收父进程的信息

int init_pool(son_t * list, int num){

    // 创建POOL_SIZE个子进程
    for (int i = 0; i < POOL_SIZE; i++){
        // 在父进程里，创建local_socket对象
        int local_socket[2];
        socketpair(AF_LOCAL, SOCK_STREAM, 0, local_socket);
        pid_t pid = fork();
        if (pid == 0){
            // 子进程：
            close(local_socket[0]);
            // 子进程的工作函数
            printf("pool:init_pool:this pid = %d, ready to go son_main!\n",getpid());
            son_main(local_socket[1]);
        } else {
            // 父进程：记录子进程的信息
            close(local_socket[1]);
            list[i].pid = pid;
            list[i].status = FREE;
            list[i].local_socket = local_socket[0];
            printf("pool:init_pool:Record son-%d info\n",i+1);
        }
        
    }

    return 0;   
}

// 有新连接进来，缓冲池分配一个线程
int to_son(son_t* list, int pool_size, int net_fd){
    printf("pool:to_son:new job comming! ready to work!\n");
    for (int i = 0; i < pool_size; i++){
        if (list[i].status == FREE){
            // 通过localsocket把net_fd交给子进程
            printf("pool:to_son:send net_fd:%d to son-%d\n",net_fd,i+1);
            local_send(list[i].local_socket, net_fd);
            list[i].status = BUSY;
            break;
        }
    }
    
    return 0;
}
