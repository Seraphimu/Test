#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

//监听端口
#define PORT 8888
//最大等待连接数
#define BACKLOG 5
//缓冲区大小
#define BUFFER_SIZE 1024

int main(void) {
    printf("=== TCP服务器启动 ===\n");
    //1. 创建socket文件描述符
    //利用socket()系统调用创建通信端点
    //AF_INET: IPV4协议族
    //SOCK_STREAM: TCP流式套接字
    //0: 使用默认协议(TCP协议)

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket创建失败");
        exit(-1);
    }
    printf("1. 创建socket成功, fd: %d\n", server_fd);

    //设置SO_REUSEADDR避免Address already in use

    int opt = 1;

    //成功时返回0, 不执行该代码块setsockopt()
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt)) {
        perror("setsockopt失败");
        exit(-1);
    }

    //准备服务器地址结构

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //3. 绑定socket到地址和端口
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof server_addr) < 0) {
        perror("bind失败");
        exit(-1);
    }
    printf("2. 绑定成功, 端口: %d\n", PORT);

    // 4. 开始监听连接
    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen失败");
        close(server_fd);
        exit(-1);
    }
    printf("3. 开始监听, 等待客户端连接\n");

    //5. 接受客户端连接
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof client_addr;
    //accept()阻塞式等待连接
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept失败");
        close(server_fd);
        exit(-1);
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof client_ip);
    printf("4. 客户端已连接: %s:%d\n", client_ip, ntohs(client_addr.sin_port));


    return 0;
}