#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <time.h>
#include <sys/wait.h>
#include <dirent.h>
#include <syslog.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <errno.h>

#define ERROR_CHECK(ret, err_val, msg) \
    do { \
        if ((ret) == (err_val)){ \
            fprintf(stderr, "[ERROR] %s:%d | %s | %s \n", __FILE__, __LINE__,(msg), strerror(errno)); \
            exit(EXIT_FAILURE); \
        } \
    } while(0)


/*常量与结构体定义 -----------------------------*/

#define POOL_SIZE 4
#define LISTEN_EVENTS 4
#define LISTEN_MAX 10
#define BUF_SIZE 128

// 忙闲状态
enum{
    BUSY,
    FREE
};
// 用来记录子进程的状态信息
typedef struct son_s{
    pid_t pid; // 进程号
    int status; // BUST/FREE
    int local_socket; // 和子进程通信用的本地socket文件描述符

}son_t;

/*函数定义 --------------------------------------*/

// 初始化进程池：启动子进程，记录子进程信息
int init_pool(son_t * list, int num);

// 初始化socket对象
int init_socket(int * socket_fd, const char* port, const char* ip);

// 向epoll对象里添加要监听的文件
int add_epoll(int epoll_fd, int fd);

// 有新连接进来，缓冲池分配一个线程
int to_son(son_t* list, int pool_size, int net_fd);

// 子进程的工作函数
int son_main(int local_socket);

// 把新连接，通过local_socket通信，交给子进程
int local_send(int local_socket, int net_fd);

int local_recv(int local_socket, int* net_fd);

// 子进程工作
int do_work(int net_fd);