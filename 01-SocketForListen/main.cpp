#include <iostream>
#include <sys/socket.h>
using namespace std;

// 创建套接字
int Socket(int __domain, int type, int protocol);

// 给套接字分配地址信息(IP地址和端口号)
int Bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);

// 把套接字转换成可接收连接的状态
int Listen(int sockfd, int backlog);

// 接受连接请求
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);


int main()
{
    cout << "Listen Socket 创建过程." << endl;
    return 0;
}
