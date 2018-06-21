#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
        puts("Time Out.");
    alarm(2);
}

void keyControl(int sig)
{
    if(sig == SIGINT)
        puts("CTRL + C Pressed.");
}

int main()
{
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keyControl);
    alarm(2);

    for(i = 0; i < 3; i++)
    {
        puts("wait...");
        sleep(100);
    }
    return 0;
}
