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
    char message1[] = "Hi";
    char message2[] = "I'm annother UDP host.";
    char message3[] = "Nice to meet you!";
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

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    /*
    sendto(sock, message1, sizeof(message1), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    sendto(sock, message2, sizeof(message2), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    sendto(sock, message3, sizeof(message3), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    */

    write(sock, message1, sizeof(message1));
    write(sock, message2, sizeof(message2));
    write(sock, message3, sizeof(message3));

    close(sock);

    return 0;
}
