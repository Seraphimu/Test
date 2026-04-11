#include "ProcessPool.h"

void downLoadTask()
{
    std::cout << getpid() << ": 下载任务\n"
              << std::endl;
    sleep(1);
}

void ioTask()
{
    std::cout << getpid() << ": IO任务\n"
              << std::endl;
    sleep(1);
}

void flushTask()
{
    std::cout << getpid() << ": 刷新任务\n"
              << std::endl;
    sleep(1);
}

void loadTaskFunc(std::vector<func_t> * out) {
    assert(out);
    out->push_back(&downLoadTask);
    out->push_back(&ioTask);
    out->push_back(&flushTask);
}