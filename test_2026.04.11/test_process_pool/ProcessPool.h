#include <iostream>
#include <vector>

#include <cassert>

#include <unistd.h>
#define MAX 1024


typedef void (*func_t)();

//A set of print()
//模拟任务
void downLoadTask();
void ioTask();
void flushTask();

//加载方法表
//使用指针类型参数，易于判空
void loadTaskFunc(std::vector<func_t> * out);


class SubEp {
public:
    SubEp(pid_t subId, int writeFd) {
        this->subId = subId;
        this->writeFd = writeFd;

        char nameBuf[MAX];
        snprintf(nameBuf, sizeof nameBuf, "n{%d}_p{%d}_w{%d}", num, getpid(), writeFd);
        this->name = nameBuf;
    }
public:
    static int num;
    std::string name;
    pid_t subId;
    int writeFd;
};

int SubEp::num = 0;