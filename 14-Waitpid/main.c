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
        sleep(15);
        return 24;
    }
    else
    {
        printf("Child Pid: %d \n", pid);

        while(!waitpid(-1, &status, WNOHANG))
        {
            sleep(3);
            puts("sleep 3sec.");
        }

        if(WIFEXITED(status))
            printf("Child send %d.\n", WEXITSTATUS(status));
    }

    return 0;
}
