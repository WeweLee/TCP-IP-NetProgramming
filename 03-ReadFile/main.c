#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void error_handling(char *err_message)
{
    fputs(err_message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main()
{
    int fd; // 文件描述符
    char buf[100];

    fd = open("data.txt", O_RDONLY);
    if(fd == -1)
        error_handling("open() error");
    printf("file descriptor: %d \n", fd);

    if(read(fd, buf, sizeof(buf)) == -1)
        error_handling("write() error.");
    printf("file data: %s", buf);
    close(fd);
    return 0;
}
