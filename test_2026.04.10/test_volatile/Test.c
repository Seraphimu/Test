#include <stdio.h>
#include <signal.h>

//O3，只改内存中的quit，没改寄存器中的quit
//保持内存可见性，保持内存与寄存器的数据一致
volatile int quit = 0;

void testGcc();
void testGccHandler(int sig);

//10个全退了，信号来不及处理
//父进程不知道还有几个子进程未退出
//课件
//手动忽略signal(SIGCHLD, SIG_IGN)仅Linux下有效
void testSIGChild();
void testSIGChildHandler(int sig);

int main(void) {
    testGcc();

    return 0;
}

void testGcc() {
    signal(2, &testGccHandler);
    while (!quit) {
        ;
    }

    printf("正常退出\n");
}

void testGccHandler(int sig) {
    printf("%d信号正在被捕捉\n", sig);
    printf("quit: %d", quit);
    quit = 1;
    printf("->%d\n", quit);
}