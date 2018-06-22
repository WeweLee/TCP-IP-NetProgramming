/****  实现并发回声服务器  ****/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1000

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_childproc(int sig)
{
    if(sig == SIGCHLD){
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        printf("Remove Proc: %d. \n", pid);
    }
}

int main(int argc, char *argv[])
{
    int serv_sock, data_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    int str_len, state;
    char buf[BUF_SIZE];

    pid_t pid;
    struct sigaction act;

    int fds[2];

    if(argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    // 注册子进程销毁回调
    act.sa_handler = read_childproc;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    state = sigaction(SIGCHLD, &act, 0);

    // 设置服务器接收端口
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error!");

    pipe(fds);
    pid = fork();
    if(pid == 0)
    {
        FILE *fp = fopen("echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        int i, len;
        for(i = 0; i < 10; i++)
        {
            len = read(fds[0], msgbuf, BUF_SIZE); // 管道里没有数据则等待,有即取出
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    while(1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        data_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if(data_sock == -1)
            continue;
        else
            puts("New Client connected...");

        pid = fork();
        if(pid == -1)
        {
            close(data_sock);
            continue;
        }
        else if(pid == 0)
        {
            // 由子进程向客户端提供回声服务
            close(serv_sock);   //可关闭服务器套接字,是因为服务器套接字文件描述符同样也传递到子进程
            while((str_len = read(data_sock, buf, BUF_SIZE)) != 0){
                write(data_sock, buf, str_len);
                write(fds[1], buf, str_len);
            }

            close(data_sock);
            puts("client disconnected...");
            return 0;
        }
        else
        {
            // 由accept函数创建的套接字文件描述符已经复制给了子进程
            close(data_sock);
        }
    }

    close(serv_sock);

    return 0;
}
