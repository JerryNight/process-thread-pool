#include "head.h"

// 本地套接字通信
// 主要实现两个函数：local_send, local_recv

// 发送信息：把新连接，通过local_socket通信，交给子进程
int local_send(int local_socket, int net_fd){
    // 创建msghdr
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));

    // 赋值正文信息
    struct iovec iov[1];
    char* str = "sendmsg";
    iov[0].iov_base = str;
    iov[0].iov_len = strlen(str);
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    // 赋值控制信息
    struct cmsghdr *cms = (struct cmsghdr*) malloc(CMSG_LEN(sizeof(int)));
    cms->cmsg_len = CMSG_LEN(sizeof(int));
    cms->cmsg_level = SOL_SOCKET;
    cms->cmsg_type = SCM_RIGHTS;

    // 获取文件描述符数组
    void* p = CMSG_DATA(cms);
    int *fd = (int*)p;
    *fd = net_fd;

    msg.msg_control = cms;
    msg.msg_controllen = CMSG_LEN(sizeof(int));

    // 发送信息
    sendmsg(local_socket, &msg, 0);

    return 0;
}

// 接收信息：从父进程那里，获取我的下一个服务对象(下一个要服务的客户端)
int local_recv(int local_socket, int* net_fd){

    // 创建msg，接收数据
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));

    // 创建正文信息
    struct iovec iov[1];
    char buf[BUF_SIZE] = {0};
    iov[0].iov_base = buf;
    iov[0].iov_len = BUF_SIZE;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    // 创建控制信息
    struct cmsghdr *cms = (struct cmsghdr*) malloc(CMSG_LEN(sizeof(int)));
    cms->cmsg_len = CMSG_LEN(sizeof(int));
    cms->cmsg_level = SOL_SOCKET;
    cms->cmsg_type = SCM_RIGHTS;

    msg.msg_control = cms;
    msg.msg_controllen = CMSG_LEN(sizeof(int));

    // 接收数据
    int ret = recvmsg(local_socket, &msg, 0);
    // printf("local_recv: resvmsg: %d", ret);

    // 获取文件描述符数组
    void* p = CMSG_DATA(cms);
    int *fd = (int*)p;

    *net_fd = *fd;

    return 0;
}
