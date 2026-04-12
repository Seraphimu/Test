#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 1024

/**
 * 创建管道的本质:
 * 1. 利用父进程创建一个管道
 * 2. 创建一个子进程, 复制父进程的地址空间. 虽然文件不会被复制, 但文件描述符表会被完整复制
 *    此时父子进程就用相同的文件描述符指向了同一个管道
 * 3. 此时释放掉父进程的读端, 子进程的写端, 或者反过来, 就可以构造一个单向读写的管道了
 */
void pipeCreate();

void pipeCreateFork();