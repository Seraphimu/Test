#include <iostream>

#include <unistd.h>
#include <sys/socket.h>

#define DEFAULT 8081

class UdpServer {
public:
    UdpServer(int port = DEFAULT) {
        this->port = port;
        this->sfd = -1;
    }

    ~UdpServer() {
        if (sfd >= 0) {
            close(sfd);
        }
    }
    void UdpServerInit() {
        //创建套接字
        //选择协议家庭: AF_INET
        //服务类型: TCP: SOCK_STREAM, UDP: SOCK_DGRAM
        //服务类别: 0根据前两个参数推出所需协议
        // On  success, a file descriptor for the new socket is returned.  On error, -1 is returned,
        sfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sfd < 0) {
            std::cerr << "socket creat failed\n";
            exit(-1);
        }
        std::cout << "socket creat success, sfd: " << sfd << "\n";
    }
private:
    int port;
    int sfd;
};


int main(void) {


    return 0;
}