#pragma once
#include <string>
#include <functional>
#include <cassert>
#include <pthread.h>

using func_t = std::function<void * (void *)>;
const int MAX = 1024;

class Thread {
private:
    std::string name;
    func_t task;
    void * args;
    pthread_t tid;
    static int threadNum;
public:
    Thread() {
        char nameBuf[MAX];
        snprintf(nameBuf, sizeof nameBuf, "T%d", threadNum++);
        this->name = nameBuf;
    }
    ~Thread() {}
    void start(func_t task, void * args = nullptr) {
        //让对象保存函数指针, 参数这些属性
        this->task = task;
        this->args = args;
        //最初, 这里传入的是this->args, 但传入nullptr为参数时出了段错误
        //最终用gdb跟踪到start_routine这一块, 不段错误才怪呢, 对0号地址解引用...
        int n = pthread_create(&tid, nullptr, &start_routine, this);
        assert(n == 0);
        (void)n;
    }

    void join() {
        int n = pthread_join(tid, nullptr);
        assert(n == 0);
        (void)n;
    }

    std::string getName() {
        return name;
    }

private:
    static void * start_routine(void * args) {
        Thread * t = static_cast<Thread *>(args);
        void * ret = t->task(t->args);
        return ret;
    }
};

int Thread::threadNum = 0;
