/****  select函数调用示例  ****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#define BUF_SIZE 30

int main()
{
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads); // 0 是标准输入(console)

    while(1)
    {
        temps = reads;  // 因为调用select函数后,除发生变化的文件描述符对应位外,剩下的所有位置0
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, 0, 0, &timeout);
        if(result == -1)
        {
            puts("select() error!");
            break;
        }
        else if(result == 0)
        {
            puts("time-out!");
        }
        else
        {
            // 验证发生变化的文件描述符是否为标准输入
            if(FD_ISSET(0, &temps))
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s \n", buf);
            }
        }
    }

    return 0;
}
