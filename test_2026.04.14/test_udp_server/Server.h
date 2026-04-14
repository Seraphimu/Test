/**
 * C/S通信 == 进程通信
 * 
 * 前提: 看到同一份资源 == 建立连接, 创建UDP套接字(大部分工作应该在这里)
 * 跨主机通信的成本绝对远大于同一主机上两个进程通信. 
 */


#include <iostream>
#include <string>
#include <string.h>
#include <cerrno>
#include <cstdlib>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace Server {
    using namespace std;
    static const string defaultIp = "0.0.0.0";
    static const int gnum = 1024;

    enum {
        USAGE_ERR = 1,
        SOCKET_ERR, 
        BIND_ERR
    };

    typedef function<void (string, uint16_t, string)> func_t;

    class UDPServer {
    public:
        UDPServer (const func_t & cb, const uint16_t &port, const string &ip = defaultIp) {
            this->callBack = cb;
            this->port = port;
            this->ip = defaultIp;
        }

        void serverInit() {
            // 1. 创建套接字
            sfd = socket(AF_INET, SOCK_DGRAM, 0);
            if (sfd == -1) {
                cerr << "socket error" << errno << ": " << strerror(errno) << "\n";
                exit(SOCKET_ERR);
            }
            cout << "socket success: " << sfd << "\n";
            // 2. 绑定port, ip
            struct sockaddr_in local;
            bzero(&local, sizeof local);
            //IPV4协议簇
            local.sin_family = AF_INET;
            //设置端口
            local.sin_port = htons(port);
            //设置IP
            local.sin_addr.s_addr = inet_addr(ip.c_str());
            
            int n = bind(sfd, (struct sockaddr *)&local, sizeof local);
            if (n == -1) {
                cerr << "bind error " << errno << ": " << strerror(errno) << "\n";
                exit(BIND_ERR);
            }
        }
    private:
        uint16_t port;
        string ip;
        int sfd;
        //什么玩意?
        func_t callBack;
    };
}