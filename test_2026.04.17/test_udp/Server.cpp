#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }
    
    // 绑定地址
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);              // 监听8888端口
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 接受所有网卡的数据
    
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }
    
    std::cout << "UDP服务器已启动，监听端口: 8888" << std::endl;
    std::cout << "等待客户端连接..." << std::endl;
    
    char buffer[1024];
    
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        
        // 接收数据
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                            (struct sockaddr*)&client_addr, &addr_len);
        
        if (n >= 0) {
            buffer[n] = '\0';
            
            // 获取客户端信息
            std::string ip = inet_ntoa(client_addr.sin_addr);
            uint16_t port = ntohs(client_addr.sin_port);
            
            std::cout << "\n收到来自 " << ip << ":" << port 
                     << " 的消息: " << buffer << std::endl;
            
            // 构造回复
            std::string reply = "服务器已收到: ";
            reply += buffer;
            
            // 发送回复
            sendto(sockfd, reply.c_str(), reply.length(), 0,
                  (struct sockaddr*)&client_addr, addr_len);
        }
    }
    
    close(sockfd);
    return 0;
}
