#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

void error_handling(char *err_message)
{
    fputs(err_message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main()
{
    int fd1, fd2, fd3; // 文件描述符

    fd1 = socket(PF_INET, SOCK_STREAM, 0);
    fd2 = open("data.txt", O_RDONLY);
    fd3 = socket(PF_INET, SOCK_STREAM, 0);

    if(fd1 == -1)
        error_handling("open() e"
                       "rror");

    if(fd2 == -1 || fd2 == -1)
        error_handling("socket() error");

    // 0,1,2是分配给标准I/O的描述符
    printf("fd1: %d \n", fd1);  // 3
    printf("fd2: %d \n", fd2);  // 4
    printf("fd3: %d \n", fd3);  // 5

    close(fd1); close(fd2); close(fd3);

    return 0;
}
