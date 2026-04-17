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
    
    // 服务器地址
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);           // 服务器端口
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 服务器IP
    
    std::string message;
    
    while (true) {
        std::cout << "请输入消息 (输入quit退出): ";
        std::getline(std::cin, message);
        
        if (message == "quit") {
            break;
        }
        
        // 发送消息
        ssize_t n = sendto(sockfd, message.c_str(), message.length(), 0,
                          (struct sockaddr*)&server_addr, sizeof(server_addr));
        
        if (n < 0) {
            perror("sendto");
            continue;
        }
        
        std::cout << "已发送 " << n << " 字节" << std::endl;
        
        // 接收回复
        char buffer[1024];
        socklen_t addr_len = sizeof(server_addr);
        
        n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                    (struct sockaddr*)&server_addr, &addr_len);
        
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "服务器回复: " << buffer << std::endl;
        }
    }
    
    close(sockfd);
    return 0;
}