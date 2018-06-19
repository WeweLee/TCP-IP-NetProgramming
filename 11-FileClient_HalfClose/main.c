#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;

    char buf[BUF_SIZE];
    int read_cnt;

    FILE *fp;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error.");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error.");
    else
        puts("Connected......");                // 送一串字符串到stdout,自动换行
        // fputs("Connected......", stdout);    // 作用同上,但不自动换行

    fp = fopen("receive.c", "wb");
    while((read_cnt = read(sock, buf, BUF_SIZE)) != 0)
        fwrite((void*)buf, 1, read_cnt, fp);

    shutdown(sock, SHUT_RD);
    puts("Received File Data.");

    write(sock, "Thank you!", 11);
    fclose(fp);

    close(sock);

    return 0;
}
