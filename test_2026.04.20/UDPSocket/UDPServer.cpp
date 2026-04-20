#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8848
#define DEFAULT_IP "0.0.0.0"
#define MAX_BUF 1024

class UDPServer {
private:
    int m_Sockfd;
    int m_Port;
    std::string m_Ip;
    enum ERROR {
        SOCKET_ERROR = 101,
        BIND_ERROR,
    };

public:
    UDPServer(int port = DEFAULT_PORT, std::string ip = DEFAULT_IP) : m_Port(port), m_Ip(ip){

    }
    void serverInit() {
        createSocket();
        struct sockaddr_in local = createAddress();
        bindAddressSocket(local);
    }
    void serverStart() {
        //在这种死循环下, 最怕内存泄露, 平时写的代码跑完就退出了, 操作系统会回收资源, 
        //但这里的进程是常驻的, 用个几年可能都不会动的, 如果它不主动归还资源, 麻烦就大了.
        struct sockaddr_in peer;
        memset(&peer, 0, sizeof peer);
        socklen_t len;
        char buf[MAX_BUF];
        while (true) {
            ssize_t r = recvfrom(m_Sockfd, buf, sizeof buf, 0, (struct sockaddr *)&peer, &len);
            if (r > 0) {
                buf[r] = 0;
                std::string clientIp = inet_ntoa(peer.sin_addr);
                uint16_t clientport = peer.sin_port;
                std::string msg = buf;
                std::cout << "[]" << clientIp << ":" << clientport << "] " << msg << "\n";
            }
        }
    }
private:
    void createSocket() {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1) {
            std::cerr << "create socket failed\n" << errno << ": " << strerror(errno) << "\n";
            exit(SOCKET_ERROR);
        }
        //走到这里, 创建成功
        // return sockfd;
        this->m_Sockfd = sockfd;
        std::cout << "socket fd: " << m_Sockfd << "\n";
    }

    struct sockaddr_in createAddress() {
        struct sockaddr_in local;
        memset(&local, 0, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = this->m_Port;
        local.sin_addr.s_addr = inet_addr(m_Ip.c_str());

        return local;
    }

    void bindAddressSocket(const struct sockaddr_in & local) {
        int n = bind(m_Sockfd, (const sockaddr *)&local, sizeof local);
        if (n == -1) {
            std::cerr << "create socket failed\n" << errno << ": " << strerror(errno) << "\n";
            exit(BIND_ERROR);
        }
    }

};

int main(void) {
    UDPServer u;
    u.serverInit();
    u.serverStart();

    return 0;
}
