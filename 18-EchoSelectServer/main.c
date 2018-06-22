/****  实现并发回声服务器  ****/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUF_SIZE 100

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serv_sock, data_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    int str_len;
    char buf[BUF_SIZE];

    struct timeval timeout;
    fd_set reads, cpy_reads;
    int fd_max, fd_num, i;

    if(argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

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

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while(1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;
        if(fd_num == 0)
            continue;

        for(i = 0; i < fd_max + 1; i++)
        {
            if(FD_ISSET(i, &cpy_reads))
            {
                if(i == serv_sock){
                    // 与服务器端连接的套接字
                    clnt_addr_size = sizeof(clnt_addr);
                    data_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

                    FD_SET(data_sock, &reads);
                    if(fd_max < data_sock)
                        fd_max = data_sock;
                    printf("connected client: %d \n", data_sock);

                }
                else
                {
                    // 与客户端连接的套接字
                    str_len = read(data_sock, buf, BUF_SIZE);
                    if(str_len == 0)
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("close client: %d \n", i);
                    }
                    else
                    {
                        write(data_sock, buf, str_len);
                    }
                }
            }
        }
    }

    close(serv_sock);

    return 0;
}
