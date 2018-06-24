/****  实现并发回声服务器  ****/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUF_SIZE 30

int acpt_sock;
int recv_sock;

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void urg_handler(int sig)
{
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s \n", buf);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    int str_len, state;
    char buf[BUF_SIZE];

    struct sigaction act;

    if(argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    // 注册子进程销毁回调
    act.sa_handler = urg_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    // 设置服务器接收端口
    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");
    if(listen( acpt_sock, 5) == -1)
        error_handling("listen() error!");

    clnt_addr_size = sizeof(clnt_addr);
    recv_sock = accept(acpt_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

    //fcntl(recv_sock, F_SETOWN, getpid());
    state = sigaction(SIGURG, &act, 0);

    while((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0)
    {
        if(str_len == -1)
            continue;
        buf[str_len] = 0;
        puts(buf);
    }

    close(acpt_sock);
    close(recv_sock);

    return 0;
}
