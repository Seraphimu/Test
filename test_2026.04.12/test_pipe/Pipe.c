#include "Pipe.h"

void pipeCreate() {
    int fds[2];
    int ret = pipe(fds);
    assert(ret == 0);

    printf("Pipe Read fds[0]: %d\n", fds[0]);
    printf("Pipe Write fds[1]: %d\n", fds[1]);
}

void pipeCreateFork() {
    int fds[2];
    int ret = pipe(fds);
    assert(ret == 0);
    (void)ret;

    printf("Pipe Read fds[0]: %d\n", fds[0]);
    printf("Pipe Write fds[1]: %d\n", fds[1]);

    int id = fork();
    assert(id >= 0);
    if (id == 0) {
        //子进程读数据
        close(fds[1]);
        char buf[MAX];
        while (1) {
            ssize_t r = read(fds[0], buf, sizeof buf);
            if (r > 0) {
                buf[r] = '\0';
            }
            printf("%s\n", buf);
            sleep(1);
        } 
    }

    //父进程写数据
    close(fds[0]);
    char buf[MAX];
    //如果buf使用指针动态分配内存的话, 这里不能直接用sizeof, 而要用MAX
    //向buf中写数据
    int cnt = 1;
    while (cnt <= 10) {
        snprintf(buf, sizeof buf, "子进程写了一份数据, 第 %d 份", cnt);
        //write()的本质, 把用户缓冲区的数据拷贝至内核缓冲区, 并适时刷向文件
        write(fds[1], buf, strlen(buf));
        sleep(1);
        cnt++;
    }

    waitpid(id, NULL, 0);

}

// #include <fcntl.h>
// void test() {
//     int fd = open("log.txt", O_RDWR);
// }