/****  用于创建僵尸进程  ****/
#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;

    pid = fork();

    // 由结果分析,调用fork函数后,父子进程拥有完全独立的内存结构
    if(pid == 0)
    {
        printf("Child Process...");
    }
    else
    {
        printf("Child Pid: %d \n", pid);
        sleep(30); // 睡眠 30 秒,经过30秒后父进程和僵尸进程同时销毁
    }

    if(pid == 0)
        printf("End child process. \n");
    else
        printf("End parent process. \n");

    return 0;
}
