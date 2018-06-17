#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 1024
#define OPSZ 4

void error_handling(char *message)
{
    // fputs向指定的文件写入一个字符串
    // stderr是Unix标准输出文件
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int calculate(int opnum, int opnds[], char op)
{
    int result = 0, i;
    switch(op)
    {
    case '+':
        for(i = 0; i < opnum; i++)result += opnds[i];
        break;
    case '-':
        for(i = 0; i < opnum; i++)result -= opnds[i];
        break;
    case '*':
        for(i = 0; i < opnum; i++)result *= opnds[i];
        break;
    }
    return result;
}

int main(int argc, char *argv[])
{
    // 指针数组 char *argv[]
    // 数组指针 char (*p)[5] 一个指针指向一个数组

    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char opinfo[BUF_SIZE];
    int str_len, i;

    int opnd_cnt, recv_cnt, recv_len;
    int result;

    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error.");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1])); // 设置监听9190端口

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error.");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error.");

    clnt_addr_size = sizeof(clnt_addr);
    for(i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1)
            error_handling("accept() error.");
        else
            printf("Client %d Connected.\n ", i);

        opnd_cnt = 0;
        read(clnt_sock, &opnd_cnt, 1);

        recv_len = 0;
        while((opnd_cnt*OPSZ + 1) > recv_len)
        {
            recv_cnt = read(clnt_sock, (int*)opinfo, BUF_SIZE - 1);
            recv_len+=recv_cnt;
        }

        result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);
        write(clnt_sock, (char*)&result, sizeof(result));

        close(clnt_sock);
    }

    close(serv_sock);

    return 0;
}
