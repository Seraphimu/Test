#include <iostream>

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static const std::string defaultIp = "0.0.0.0";
static const uint16_t defaultPort = 8888;
static const int gNum = 1024;

using func_t = std::function<void (std::string, uint16_t, std::string)>;
enum {USAGE_ERR = 1, SOCKET_ERR, BIND_ERR};

class UDPServer {
private:
    uint16_t port;
    std::string ip;
    int sockfd;
    func_t callBack;

public:
    UDPServer(const func_t callBack, uint16_t port = defaultPort, std::string ip = defaultIp) : 
    callBack(callBack), port(port), ip(ip), sockfd(-1) { }
    
    void initServer() {
        //创建套接字, 拿到文件fd
        sockfd = createSocket();
        //创建地址, 用于绑定socket
        //"如果有数据发送到这个IP:Port，就交给我这个socket处理"
        struct sockaddr_in local = createAddress();
        bindSocketAddress(local);
        //创建套接字并绑定的预备工作完成!
    }
    void start() {
        char buffer[gNum];
        while (true) {
            struct sockaddr_in peer;
            memset(&peer, 0, sizeof peer);
            socklen_t len = sizeof peer;
            //buffer: 读数据的地方
            //len: 真实地址的大小, 如果小了不会越界访问, 它只会截断并放入peer, 然后再返回正确的len
            ssize_t s = recvfrom(sockfd, buffer, sizeof (buffer) - 1, 
                0, (struct sockaddr*)&peer, &len);
            if (s > 0) {
                buffer[s] = 0;
                std::string clientIp = inet_ntoa(peer.sin_addr);
                uint16_t clientPort = ntohs(peer.sin_port);
                std::string message = buffer;
                std::cout << clientIp << "[" << clientPort << "]#" << message << "\n";
                callBack(clientIp, clientPort, message);
            }
        }
    }

private:
    //创建一个地址
    //第一次感觉到了封装的便利.
    struct sockaddr_in createAddress() {
        struct sockaddr_in local;
        //应该是类似于memset的函数
        bzero(&local, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        //这里的ip全0不是公网ip, 而是指其他ip访问的自己本地的网卡ip
        //它解决的是服务器有多张“网卡”（多个IP）时，监听哪个“本地地址”的问题，而不是过滤“远程地址”的问题。​
        //像是告诉邮局：“寄到我家任何一个收件地址（如公司地址、家庭地址, 这里是网卡）的信，都交给我”
        local.sin_addr.s_addr = inet_addr(ip.c_str());
        return local;
    }

    int createSocket() {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1) {
            std::cerr << "socker error: " << errno << strerror(errno) << "\n";
            exit(SOCKET_ERR);
        }
        std::cout << "socket success, sockfd: " << sockfd << "\n";
        return sockfd;
    }

    void bindSocketAddress(struct sockaddr_in local) {
        int n = bind(sockfd, (struct sockaddr*)&local, sizeof local);
        if (n == -1) {
            std::cerr << "bind error: " << errno << " : " << strerror(errno) << '\n';
            exit(BIND_ERR);
        }
    }
};

int main(void) {
    UDPServer udp(nullptr);
    udp.initServer();

    return 0;
}