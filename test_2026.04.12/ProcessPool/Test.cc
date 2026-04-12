/**
 * 核心思想: 创建PROCESS_NUM个管道和子进程, 让父进程通过唯一一个管道与子进程进行通信.
 * 父进程发送指令下标, 子进程读取下标并执行指令
 */

#include "ProcessPool.h"

int SubEp::num = 0;

int main(void) {
    MAKE_SEED();
    //func_t: 指向一个参数空返回void的函数
    std::vector<func_t> task;
    loadTaskFunc(&task);

    //创建一批子进程, 用于接收父进程的命令码, 通过管道
    std::vector<SubEp> subs;
    createSubProcess(&subs, task);

    // 3. 父进程作为负载均衡控制器，发送任务
    int taskCnt = 3;  // 发送3个任务，0表示永远发送
    loadBalanceControl(subs, task, taskCnt);

    waitProcess(subs);

    return 0;
}