#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX 64

void testDupStdin();
void testDupStdout();
void guess();

int main(void) {
    // printf("Fuck You World!\n");
    // testDupStdin();
    // testDupStdout();
    guess();

    return 0;
}

//把文件内容读出来，并输入到stdin中
//再通过fgets()函数读出来
//输入重定向：把stdin换成指定文件的fd，把文件内容输入到stdin中
void testDupStdin() {
    int fd = open("log.txt", O_RDONLY);
    assert(!(fd < 0));

    //输入重定向，把fd下标指向的文件填入0下标，即stdin
    dup2(fd, 0);
    //此时对fd下标文件的操作，就变成了对0下标文件的操作
    
    char line[MAX];

    while (1) {
        printf(">");
        //至多读入sizeof line - 1个字符存入line中，并在最后补上\0
        //读到换行符就放进来
        if (fgets(line, sizeof line, stdin) == NULL) {
            break;
        }
        printf("%s", line);
    }

    close(fd);
}

void testDupStdout() {
    umask(0);
    //自发问：如果重定向了，原来这个fd会不会还继续指向该文件呢？
    //验证：再打开一个文件，看看fd是几就知道了
    //答案是4，它还继续指向那份文件
    int fd = open("log_out.txt", O_WRONLY | O_CREAT, 0666);
    assert(fd >= 0);
    printf("log_out.txt fd: %d\n", fd);

    //让fd指向的文件接管stdout标准输出流
    //即：原本输出到终端上的数据会输入到该文件中
    dup2(fd, 1);

    //验证重定向后，fd还是否指向指定的文件
    // int fd2 = open("log.txt", O_RDONLY);
    // assert(fd2 >= 0);
    // printf("log.txt fd: %d\n", fd2);
    // close(fd2);

    for (int i = 0; i < 10; i++) {
        printf("这是第%d次打印\n", i+1);
    }



    close(fd);
}

/**
 * 猜一猜会输出什么？
 * 如果执行了a.out > myfile.txt, 就会看到C接口的hello被输出了两次，而write()系统调用被输出了一次
 * 如果直接执行a.out则正常输出
 * 这是因为缓冲区的刷新策略问题。C语言提供三种缓冲区刷新策略：
 * 1. 无缓冲，即来即刷
 * 2. 行缓冲，换行即刷
 * 3. 全缓冲，缓冲区满即刷
 * 除此之外还有两种强制刷新策略：
 * 1. 用户通过fflush()系统调用强制刷新
 * 2. 进程退出时强制刷新
 * stdout即标准输出默认使用行缓冲刷新策略来刷新数据，也就是说，
 * 在C语言提供的缓冲区中，流向stdout的数据流中，只要有一个'\n'，
 * 那么'\n'即之前的数据会被从缓冲区拿掉，刷新到文件中
 * 
 * 而向文件写入，C语言默认使用全缓冲，即缓冲区满才刷新。而hello prinf这样的字符串显然不足矣让缓冲区满
 * 而用户也没有强制刷新，所以只有在进程退出时才会被刷新
 * 而在进程退出之前，该进程被fork()，子进程拷贝了父进程的 `task_struct`, `mm_struct`等资源，
 * 而语言提供的缓冲区就维护在父进程的地址空间中，它们一块被拷贝了过来，
 * 直到进程退出时，父子进程的C缓冲区中都有数据，所以会一齐刷出。
 * 而write()系统调用的本质是：把数据从用户缓冲区:wstring复制到内核缓冲区中，然后在适当的时候写入到文件
 * 而fork()是不会复制内核区的，所以write只输出一份
 */
void guess() {
    // C接口
    printf("hello printf\n");
    fprintf(stdout, "hellp fprintf\n");
    const char *fputsString = "hello fputs\n";
    fputs(fputsString, stdout);


    // 系统调用接口
    const char *wstring = "hello write\n";
    write(1, wstring, strlen(wstring));

    fork();
}