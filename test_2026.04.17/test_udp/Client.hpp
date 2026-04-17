#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// 简化版UDP客户端
class UDPClient {
private:
    int sockfd;                 // 套接字文件描述符
    std::string server_ip;      // 服务器IP
    uint16_t server_port;       // 服务器端口
    struct sockaddr_in server_addr;  // 服务器地址结构
    
public:
    UDPClient(const std::string& ip, uint16_t port) 
        : server_ip(ip), server_port(port), sockfd(-1) {
        memset(&server_addr, 0, sizeof(server_addr));
    }
    
    ~UDPClient() {
        if (sockfd != -1) {
            close(sockfd);
        }
    }
    
    // 初始化客户端
    bool init() {
        // 创建socket
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket创建失败");
            return false;
        }
        std::cout << "✓ Socket创建成功，fd: " << sockfd << std::endl;
        
        // 设置服务器地址
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(server_port);
        server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
        
        return true;
    }
    
    // 发送消息
    bool send(const std::string& message) {
        ssize_t n = sendto(sockfd, message.c_str(), message.length(), 0,
                          (struct sockaddr*)&server_addr, sizeof(server_addr));
        
        if (n < 0) {
            perror("发送失败");
            return false;
        }
        
        std::cout << "✓ 已发送 " << n << " 字节" << std::endl;
        
        // 尝试接收回复
        char buffer[1024];
        socklen_t addr_len = sizeof(server_addr);
        
        n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                    (struct sockaddr*)&server_addr, &addr_len);
        
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "📨 收到回复: " << buffer << std::endl;
        }
        
        return true;
    }
};