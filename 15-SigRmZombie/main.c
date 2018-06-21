/****  用于创建僵尸进程  ****/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    if(sig == SIGCHLD){
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if(WIFEXITED(status))
        {
            printf("Remove proc id: %d \n", pid);
            printf("Child send: %d \n", WEXITSTATUS(status));
        }
    }
}

int main()
{
    pid_t pid;
    struct sigaction act;
    int i;

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGCHLD, &act, 0);

    pid = fork();

    // 由结果分析,调用fork函数后,父子进程拥有完全独立的内存结构
    if(pid == 0)
    {
        sleep(10);
        return 1;
    }
    else
    {
        printf("Child Pid: %d \n", pid);
        pid = fork();
        if(pid == 0)
        {
            sleep(5);
            return 2;
        }
        else
        {
            printf("Child Pid: %d \n", pid);
            for(i = 0; i < 3; i++)
            {
                puts("wait...");
                sleep(5);
            }
        }

    }

    return 0;
}
