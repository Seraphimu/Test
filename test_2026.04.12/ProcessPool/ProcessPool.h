#include <iostream>
#include <vector>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#define MAX 1024
#define PROCESS_NUM 100000
#define MAKE_SEED() srand((unsigned)time(nullptr) ^ getpid())

typedef void (*func_t)();

class SubEp {
public:
    SubEp(pid_t subId, int writeFd) {
        this->subId = subId;
        this->writeFd = writeFd;

        //构建子进程的名称
        char nameBuf[MAX];
        snprintf(nameBuf, sizeof nameBuf, "n{%d}_p{%d}_w{%d}", num++, subId, writeFd);
        this->name = nameBuf;
    }
public:
    static int num;
    std::string name;
    pid_t subId;
    //父进程的写端文件描述符
    int writeFd;
};



//A set of print()
//模拟任务
void downLoadTask();
void ioTask();
void flushTask();

//加载方法表
//使用指针类型参数，易于判空
void loadTaskFunc(std::vector<func_t> * out);

/***
 * 创建子进程, 
 */
void createSubProcess(std::vector<SubEp> * subs, std::vector<func_t> & funcMap);

int recvTask(int readFd);
void sendTask(const SubEp &process, int taskNum);

void loadBalanceControl(const std::vector<SubEp> & subs, 
    const std::vector<func_t> & funcMap, int cnt);

void waitProcess(const std::vector<SubEp> & subs);