#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class UDPClient {
private:
    int m_Sockfd;
    uint16_t m_ServerPort;
    std::string m_ServerIP;
    bool m_Quit;
public:
    UDPClient(const std::string & serverIp, const uint16_t & serverPort) : 
    m_ServerIP(serverIp), m_ServerPort(serverPort)
    {    }

    void init() {
        createSocket();
    }
    void run() {
        struct sockaddr_in server = createServerAddress();
        std::string msg = "0";
        while (!m_Quit) {
            std::cout << "[Please Enter #]";
            std::cin >> msg;
            sendto(m_Sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr *)&server, sizeof server);
        }
    }
private:
    void createSocket() {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd ==  -1) {
            std::cerr << "create socket failed\n" << errno << ": " << strerror(errno) << '\n';
            exit(-1);
        }
        m_Sockfd = sockfd;
    }
    struct sockaddr_in createServerAddress() {
        struct sockaddr_in server;
        memset(&server, 0, sizeof server);
        server.sin_family = AF_INET;
        server.sin_port = m_ServerPort;
        server.sin_addr.s_addr = inet_addr(m_ServerIP.c_str());

        return server;
    }
};


int main(int argc, char * argv[]) {
    printf("sb client\n");
    // std::cout << argv[1] << " ip" << " port\n";
    if (argc < 2) {
        //这是bug, 但测出来了, 如果argv[1]是空的, 那么后面的是打不出来的.
        // std::cout << argv[1] << " ip" << " port\n";
        printf("%s server_ip server_port\n", argv[0]);
        fflush(stdout);
        exit(-1);
    }
    UDPClient u(argv[1], atoi(argv[2]));
    u.init();
    u.run();

    return 0;
}
