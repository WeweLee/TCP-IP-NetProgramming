#include <stdio.h>
#include <arpa/inet.h>

int main()
{
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    // %x以十六进制输出, %o以八进制输出
    // %#会在输出的十六进制数前加0x
    printf("Host Port: %#x \n", host_port);
    printf("Net Port:  %#x \n", net_port);
    printf("Host Addr: %#lx \n", host_addr);
    printf("Net Addr:  %#lx \n", net_addr);

    return 0;
}
