/****  用于创建僵尸进程  ****/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int status;
    pid_t pid = fork();

    // 由结果分析,调用fork函数后,父子进程拥有完全独立的内存结构
    if(pid == 0)
    {
        return 3;
    }
    else
    {
        printf("Child 1 Pid: %d \n", pid);

        pid = fork();
        if(pid == 0)
            exit(7);
        else
        {
            printf("Child 2 pid: %d \n", pid);
            wait(&status);
            if(WIFEXITED(status))
                printf("Child 1 Send: %d \n", WEXITSTATUS(status));

            wait(&status);
            if(WIFEXITED(status))
                printf("Child 2 Send: %d \n", WEXITSTATUS(status));
            // 使用wait函数后,没有僵尸Zimbie进程
            sleep(30);
        }
    }
    return 0;
}
