#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//默认端口, 缓冲区大小
#define DEFAULT_PORT 8888
#define BUF_MAX 1024

int main(void) {
    //创建文件fd
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sockfd >= 0);

    //构造ip地址
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;                  //设置协议类型, 是IPV4还是IPV6
    addr.sin_port = htons(DEFAULT_PORT);        //设置端口
    addr.sin_addr.s_addr = htonl(INADDR_ANY);   //让任意IP地址者可以访问? 
    
    //绑定文件fd与ip:端口, 收数据的还是发数据的? 收数据的
    int n = bind(sockfd, (struct sockaddr *)&addr, sizeof addr);
    assert(n == 0);

    std::cout << "UPD服务端启动, 监听: " << addr.sin_port << "\n";
    std::cout << "等待客户端连接...\n";

    char buf[BUF_MAX];

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof client_addr;

        ssize_t n = recvfrom(sockfd, buf, sizeof buf - 1,
             0, (struct sockaddr *)&client_addr, &addr_len);
        if (n >= 0) {
            buf[n] = 0;
            std::string ip = inet_ntoa(client_addr.sin_addr);
            uint16_t port = ntohs(client_addr.sin_port);
            std::cout << "\n收到来自" << ip << ":" << port << 
            "的消息: " << buf << "\n";

            std::string replay = "服务器收到: ";
            replay += buf;

            sendto(sockfd, replay.c_str(), replay.length(), 
                0, (struct sockaddr *)&client_addr, addr_len);
        }
    }

    close(sockfd);

    return 0;
}