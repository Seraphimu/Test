#include <iostream>
#include <string>
#include <strings.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class UDPClient {
private:
    int sockfd;
    std::string serverIp;
    uint16_t serverPort;
    bool quit;
public:
    UDPClient(const std::string & serverIp, const uint16_t & serverPort) :
    serverIp(serverIp), serverPort(serverPort) {}

    void initClient() {
        sockfd = createSocket();
    }
    void run() {
        struct sockaddr_in server = createAddress();
        std::string message;
        while (!quit) {
            std::cout << "Please Enter# ";
            std::cin >> message;
            sendto(sockfd, message.c_str(), message.size(), 
                0, (struct sockaddr *)&server, sizeof server);
            
        }
    }
private:
    int createSocket() {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1) {
            std::cerr << "socket error: " << errno << strerror(errno) << '\n';
            exit(2);
        }
        return sockfd;
    }
    struct sockaddr_in createAddress() {
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(serverIp.c_str());
        server.sin_port = htons(serverPort);
        return server;
    }
};

static void Usage(std::string proc)
{
    std::cout << "\nUsage:\n\t" << proc << " server_ip server_port\n\n";
}

// ./udpClient server_ip server_port
int main(int argc, char *argv[])
{


    return 0;
}