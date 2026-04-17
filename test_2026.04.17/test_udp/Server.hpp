#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// 简化版UDP服务器
class UDPServer {
private:
    int sockfd;                 // 套接字文件描述符
    uint16_t port;              // 监听端口
    std::string ip;             // 绑定的IP地址
    struct sockaddr_in server_addr;  // 服务器地址结构
    
public:
    UDPServer(uint16_t port, const std::string& ip = "0.0.0.0") 
        : port(port), ip(ip), sockfd(-1) {
        memset(&server_addr, 0, sizeof(server_addr));
    }
    
    ~UDPServer() {
        if (sockfd != -1) {
            close(sockfd);
        }
    }
    
    // 初始化服务器
    bool init() {
        // 1. 创建socket
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket创建失败");
            return false;
        }
        std::cout << "✓ Socket创建成功，fd: " << sockfd << std::endl;
        
        // 2. 设置地址结构
        server_addr.sin_family = AF_INET;                // IPv4
        server_addr.sin_port = htons(port);              // 端口，转换为网络字节序
        server_addr.sin_addr.s_addr = inet_addr(ip.c_str());  // IP地址
        
        // 3. 绑定socket
        if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("绑定失败");
            close(sockfd);
            return false;
        }
        std::cout << "✓ 绑定 " << ip << ":" << port << " 成功" << std::endl;
        
        return true;
    }
    
    // 开始接收数据
    void start() {
        char buffer[1024];
        
        std::cout << "🚀 UDP服务器已启动，等待数据..." << std::endl;
        
        while (true) {
            struct sockaddr_in client_addr;  // 客户端地址
            socklen_t addr_len = sizeof(client_addr);
            
            // 接收数据
            ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                                (struct sockaddr*)&client_addr, &addr_len);
            
            if (n > 0) {
                buffer[n] = '\0';  // 确保字符串以null结尾
                
                // 获取客户端信息
                std::string client_ip = inet_ntoa(client_addr.sin_addr);
                uint16_t client_port = ntohs(client_addr.sin_port);
                
                std::cout << "\n📨 收到来自 " << client_ip << ":" << client_port 
                         << " 的消息 (" << n << "字节):\n";
                std::cout << "   " << buffer << std::endl;
                
                // 简单回显
                sendto(sockfd, buffer, n, 0, 
                      (struct sockaddr*)&client_addr, addr_len);
            }
        }
    }
};








// #pragma once
// #include <iostream>
// #include <string>
// #include <cstring>
// #include <cassert>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>

// class UDPServer {
// private:
//     int fd;
//     uint16_t port;
//     std::string ip;
//     struct sockaddr_in server_addr; 
// public:
//     UDPServer(uint16_t port, const std::string & ip = "0.0.0.0") 
//     : port(port), ip(ip), fd(-1) {
//         memset(&server_addr, 0, sizeof server_addr);
//     }
//     ~UDPServer() {
//         if (fd != -1) {
//             close(fd);
//         }
//     }
//     bool init() {
//         fd = socket(AF_INET, SOCK_DGRAM, 0);
//         assert(fd >= 0);
//         std::cout << "socket, construction complete! fd = " << fd << "\n";

//         //设置地址结构
//         server_addr.sin_family = AF_INET;   //IPV4
//         server_addr.sin_port = htons(port); //端口, 转化成网络字节序
//         server_addr.sin_addr.s_addr = inet_addr(ip.c_str());    //ip地址

//         int n = bind(fd, (struct sockaddr *)&server_addr, sizeof server_addr);
//         assert(n == 0);

//         std::cout << "绑定成功, 端口: " << port << "\n";

//         return true;
//     }
//     void start() {
//         char buf[1024];
//         std::cout << "🚀 UDP服务器已启动, 等待数据...\n";
//         while ( true) {
//             struct sockaddr_in cliend_addr;
//             socklen_t addr_len = sizeof(cliend_addr);
//             ssize_t n = recvfrom(fd, buf, sizeof buf - 1,
//                  0, (struct sockaddr *)&cliend_addr, &addr_len);
            
//         }
        
//     }
// };