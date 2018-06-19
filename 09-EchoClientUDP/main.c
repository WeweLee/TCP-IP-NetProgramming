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
    struct sockaddr_in serv_addr, from_addr;
    socklen_t from_addr_size;
    char message[BUF_SIZE];
    int str_len = 0;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error.");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    while(1)
    {
       fputs("Input message(Q to quit):", stdout);
       // 输入的字符串长度超过了BUF_SIZE的长度该如何?
       fgets(message, BUF_SIZE, stdin);
       if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
           break;

       sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
       from_addr_size = sizeof(from_addr);
       str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&from_addr, &from_addr_size);
       message[str_len] = 0;
       printf("Message from server: %s \n", message);
    }

    close(sock);

    return 0;
}
