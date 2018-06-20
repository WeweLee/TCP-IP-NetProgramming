#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handing(char *msg)
{
    fputs(msg, stderr);
    fputs("\n", stderr);
    exit(1);
}

int main()
{
    int sock;
    int snd_buf = 1024*4, rcv_buf = 1024*4;
    int state;
    socklen_t len;

    printf("In: Input buffer size: %d \n", rcv_buf);
    printf("In: Output buffer size: %d \n", snd_buf);

    sock = socket(PF_INET, SOCK_STREAM, 0);

    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
    if(state)
        error_handing("setsockopt() error.");

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
    if(state)
        error_handing("setsockopt() error.");

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if(state)
        error_handing("getsockopt() error.");

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if(state)
        error_handing("getsockopt() error.");

    printf("Out: Input buffer size: %d \n", rcv_buf);
    printf("Out: Output buffer size: %d \n", snd_buf);

    return 0;
}
