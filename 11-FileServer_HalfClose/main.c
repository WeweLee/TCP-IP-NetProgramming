#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 30

void error_handling(char *message)
{
    // fputs向指定的文件写入一个字符串
    // stderr是Unix标准输出文件
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    // 指针数组 char *argv[]
    // 数组指针 char (*p)[5] 一个指针指向一个数组

    int serv_sock, data_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    FILE *fp;int read_cnt;
    char buf[BUF_SIZE];

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
    serv_addr.sin_port=htons(atoi(argv[1])); // 设置监听的端口号

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error.");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error.");

    clnt_addr_size = sizeof(clnt_addr);
    data_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(data_sock == -1)
        error_handling("accept() error.");

    fp = fopen("file_server.c", "rb");
    while(1)
    {
        // fread(void* buf, size_t size, size_t count, FILE *stream)
        // 读 count 个数据项,每个数据项 size 个字节
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if(read_cnt < BUF_SIZE)
        {
            write(data_sock, buf, read_cnt);
            break;
        }
        write(data_sock, buf, BUF_SIZE);
    }

    shutdown(data_sock, SHUT_WR);
    read(data_sock, buf, BUF_SIZE);
    printf("Message from client: %s \n", buf);

    fclose(fp);
    close(data_sock);
    close(serv_sock);

    return 0;
}
