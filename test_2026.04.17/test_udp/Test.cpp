#include "Server.hpp"

int main(void) {
	UDPServer u(8888);
	u.init();

	return 0;
}


// #include <iostream>
// #include <cstring>
// #include <fcntl.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #define DEFAULT_PORT 8888

// class UDPServer {
// private:
// 	int port;
// 	int fd;
// public:
// 	UDPServer(int port = DEFAULT_PORT) : 
// 	port(port), fd(-1) {	
// 		fd = socket(AF_INET, SOCK_DGRAM, 0);
// 		if (fd < 0) {
// 			std::cerr << "socket create failed\n";
// 			exit(-1);
// 		}
// 		std::cout << "socket creat success, fd: " << fd << "\n";
// 	}
// 	~UDPServer() {
// 		if (fd >= 0) {
// 			close(fd);
// 		}
// 	}
// 	void serverInit() {
// 		/**
// 		 *        sockaddr_in
//               Describes an IPv4 Internet domain socket address.
//               .sin_port and .sin_addr are stored in network byte order.
// 		 */
// 		struct sockaddr_in local;
// 		memset(&local, 0, sizeof local);
// 		local.sin_family = AF_INET;
// 		local.sin_port = htons(port);
// 		local.sin_addr.s_addr = INADDR_ANY;

// 		//On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error.
// 		if ((bind(fd, (sockaddr *)&local, sizeof (local)) == -1)) {
// 			std::cerr << "bind failed\n";
// 			exit(-1);
// 		}
// 		std::cout << "bind port success, port: " << port << "\n";
// 	}
// };


// int main(void) {

// 	UDPServer u(DEFAULT_PORT);
// 	u.serverInit();

// 	return 0;
// }