#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main()
{
    pid_t pid;
    int lval = 20;

    pid = fork();
    if(pid == 0)
    {
        gval += 10;
        lval += 10;
    }
    else
    {
        gval -= 10;
        lval -= 10;
    }

    // 由结果分析,调用fork函数后,父子进程拥有完全独立的内存结构
    if(pid == 0)
    {
        printf("Child Proc: [%d %d] \n", gval, lval);
    }
    else
    {
        printf("Parent Proc: [%d %d] \n", gval, lval);
    }

    return 0;
}
