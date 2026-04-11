#include "ProcessPool.h"

int main(void) {
    //func_t: 指向一个参数空返回void的函数
    std::vector<func_t> task;
    loadTaskFunc(&task);

    task[0]();
    task[1]();
    task[2]();

    return 0;
}