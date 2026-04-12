#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX 1024    

int main() {
    int fds[2];
    int ret = pipe(fds);
    assert(ret == 0);
    (void)ret;

    int id = fork();
    assert(id >= 0);
    if (id == 0) {
        //子进程关掉写端, 读数据
        close(fds[1]);
        char buf[MAX];
        int cnt = 1;
        while (cnt <= 5) {
            ssize_t r = read(fds[0], buf, sizeof buf);
            if (r > 0) {
                buf[r] = '\0';
            }
            printf("子进程收到: %s\n", buf);
            sleep(1);
            cnt++;
        } 
    }
    else {
        //父进程关闭读端, 写数据
        close(fds[0]);
        char buf[MAX];
        //如果buf使用指针动态分配内存的话, 这里不能直接用sizeof, 而要用MAX
        //向buf中写数据
        int cnt = 1;
        while (cnt <= 5) {
            snprintf(buf, sizeof buf, "父进程写了第 %d 份", cnt);
            //write()的本质, 把用户缓冲区的数据拷贝至内核缓冲区, 并适时刷向文件
            write(fds[1], buf, strlen(buf));
            sleep(1);
            cnt++;
        }

        waitpid(id, NULL, 0);
    }

    return 0;
}


// /**
//  * 创建一个管道文件, 再创建一个子进程, 并打印父子进程的fds[]
//  * 
//  * 进程是独立的内存空间，但代码是共享的（只读）
// fork() 创建的是进程，不是线程，但代码段是相同的
// if-else 是控制流，不是内存隔离

// 父子进程代码段相同, 如果子进程不在if (id == 0)时退出, 会继续向下执行.

// 踩坑: 这里多打印了:

// Child: 103190 Pipe Read fds[0]: 3
// Child: 103190 Pipe Write fds[1]: 4
// Parent: 103190 Pipe Read fds[0]: 3
// Parent: 103190 Pipe Write fds[1]: 4
// Parent: 103189 Pipe Read fds[0]: 3
// Parent: 103189 Pipe Write fds[1]: 4

// 站在父进程的角度就好理解了, 这段代码, 父进程是绝对不会执行的, 
// 所以它的目的并不是让子进程只执行这一块, 而是让父进程别执行这一块是吧.

// 所以, fork执行之后, 内存中又多了一个一模一样的进程, 不过在这个进程中, id == 0
// if (id == 0)不过是只有在子进程中才会执行而已, 其余完全一致, 这取决于fork的位置, 
// 子进程是从fork之后开始执行的, 且在子进程中fork的返回值是0

// 那么为什么是fork之后呢, 不难理解. fork做了什么? 拷贝进程地址空间, 
// 那么进程地址空间中包括了代码段, 数据段, 堆栈段, 还有其他的上下文信息
// 而程序执行的本质是什么? CPU取指令, 分析指令, 执行指令. 而取指令依赖的是什么? 
// 程序计数器, 即PC指针. 它的作用就是存放下一条要被执行的指令的虚拟地址
// 所以我们要站在机器的角度上去理解, fork之后, 进程上下文被复制, 
// 那么PC指针的值也在上下文中, 它要不要被复制? 答案是肯定的.
// 父进程执行到了fork这里, 所以PC指针一定指向fork之后的那条指令
// 而此时, 父进程被fork, 上下文被完整拷贝, 那么此时子进程的PC值要不要和父进程一致?
// 答案是肯定的.

// 计算机体系结构和操作系统内核的角度理解了fork()的本质

// ✅ fork() 复制进程地址空间（代码、数据、堆栈）
// ✅ 复制包括PC在内的所有上下文信息
// ✅ 子进程的PC值与父进程相同，指向fork后的指令
// ✅ 内核设置不同的返回值（父进程：子进程PID，子进程：0）
// ✅ 调度器决定哪个进程先运行




// 我发现结果与我预想的相关甚远，为什么会有6行输出。后来不断尝试，用了进程待，没用，给 `if` 判断加上 `else` 发现逻辑对了，或者在 `if` 内加上 `exit()` 也对了。最后我才想到打印PID来看个究竟，我发现是子进程多打印了一份本该由父进程打印的字符串。

// 这我可就奇怪了，我对 `fork()` 函数的认知就是执行 `if (id == 0)` 里的内容啊，为什么它会打印父进程的东西呢？后来经过一番调查、问AI、思考，终于得出结论：父子进程的代码段是完全相同的，而唯一的区别就是 `fork()` 返回值的不同！那段 `if (id == 0)` 并不是用来写子进程的代码的，它是一段**在父进程下永远都不执行，且在子进程下永远都会执行的代码**。很抽象？不！很具体了。

// `fork()` 之后操作系统中有两个代码段完全相同的进程，它们都在运行着，只是父进程中 `fork()` 的返回值是正整数（正常情况），所以不执行 `if` 判断中的内容，而子进程反之。但是！`if` 判断结束之后呢？后面那两条 `printf()` 语句也被执行，这就不奇怪了。因为子进程和父进程是一样的，没毛病啊！为什么不能执行？在父进程视角下，这段代码要执行没有任何条件，子进程视角下也一样。

// 所以一定要控制好 `fork()` 的位置，那为什么子进程是从 `fork()` 之后开始执行呢？这就要问一下 `fork()` 做了什么了，它最关键的一步就是拷贝，拷贝了 `task_struct`, `mm_struct` 等重要的进程数据结构。现在，我们需要站在计算机体系结构的视角下来理解，而不是单站在语言的角度上了。

// 计算机它做什么？CPU它做什么？取指、译码、执行。而取指需要什么？PC指针，即程序计数器。好，那么PC指针的值是不是进程的上下文？是的！那么它会会被保存在哪里？PCB，即 `task_struct` 中。而 `fork()` 复制了什么？`task_struct` 等重要的进程数据！那么上下文会不会被复制？PC指针要不要被复制？答案是绝对肯定的！那么在父进程执行 `fork()` 时，它的PC指针指向哪里？一定在 `fork()`的下一条指令！那么子进程的PC指针在哪里？与父进程一致！所以，子进程总是从 `fork()` 之后开始执行！

// 推理完毕！
//  */
// #include <stdio.h>
// #include <assert.h>
// #include <unistd.h>
// #include <fcntl.h>

// int main(void) {
//     int fds[2];
//     int ret = pipe(fds);
//     assert(ret == 0);

//     int id = fork();
//     assert(id >= 0);

//     //子进程
//     if (id == 0) {
//         printf("Child: %d Pipe Read fds[0]: %d\n", getpid(), fds[0]);
//         printf("Child: %d Pipe Write fds[1]: %d\n", getpid(), fds[1]);
//     }
//     else {
//         //父进程
//         printf("Parent Pipe Read fds[0]: %d\n", fds[0]);
//         printf("Parent Pipe Write fds[1]: %d\n", fds[1]);
//     }


//     return 0;
// }

// /**
//  * **实例:** 让进程自己与自己通信, 写端负责向管道里写内容, 
//  * 而读端负责将内容读出, 并打印在显示器上
//  */
// #include <stdio.h>
// #include <stdlib.h>
// #include <assert.h>
// #include <string.h>
// #include <unistd.h>
// #include <fcntl.h>

// #define MAX 128

// int main(void) {
//     int fds[2];
//     int ret = pipe(fds);
//     assert(ret == 0);

//     int cnt = 1;
//     char * bufWrite = (char *)malloc(sizeof (char) * MAX);
//     char * bufRead = (char *)malloc(sizeof (char) * MAX);

//     while (cnt <= 5) {
//         //向bufWrite缓冲区中格式化写入自定义的字符串
//         snprintf(bufWrite, MAX, "第%d条消息", cnt++);
//         //将bufWrite中的内容写入到管道
//         write(fds[1], bufWrite, strlen(bufWrite));

//         //读出管道中的内容写入到bufRead缓冲区中
//         ssize_t r = read(fds[0], bufRead, MAX -1);
//         if (r > 0) {
//             //读取成功, 在读到的最后一个字符后加上'\0'防止C/C++无法识别到结尾
//             bufRead[r] = '\0';
//         }
//         //打印
//         printf("%s\n", bufRead);
//         //慢一点
//         sleep(1);
//     }


//     close(fds[0]);
//     close(fds[1]);
//     free(bufWrite);
//     free(bufRead);

//     return 0;
// }


// // **实例:** 在进程中创建一个管道, 并打印它的读写端文件描述符.
// #include <stdio.h>
// #include <assert.h>
// #include <unistd.h>

// int main(void) {
//     int fds[2];
//     int ret = pipe(fds);
//     assert(ret == 0);

//     printf("Pipe Read fds[0]: %d\n", fds[0]);
//     printf("Pipe Write fds[1]: %d\n", fds[1]);

//     return 0;
// }



// #include "Pipe.h"

// int main(void) {
//     // pipeCreate();
//     pipeCreateFork();

//     return 0;
// }
