#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

int main()
{
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x10203040);
    addr2.sin_addr.s_addr = htonl(0x10101010);

    str_ptr = inet_ntoa(addr1.sin_addr);    // 返回的是指针
    strcpy(str_arr, str_ptr);
    printf("1:str_ptr: %s \n", str_ptr);
    printf("1:str_arr: %s \n", str_ptr);


    inet_ntoa(addr2.sin_addr);              // str_ptr指向的内存空间数据改变
    printf("2:str_ptr: %s \n", str_ptr);
    printf("2:str_arr: %s \n", str_arr);

    free(str_ptr);

    return 0;
}
