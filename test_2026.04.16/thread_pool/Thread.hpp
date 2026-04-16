#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <functional>
#include <pthread.h>

namespace ThreadNs {

    //构造一个函数对象: 返回类型void *, 参数void *
    typedef std::function<void * (void *)> func_t;
    //定义一个缓冲区最大值
    const int num = 1024;

    class Thread {
    private:
        std::string name;       //线程对象的名字
        func_t task;            //任务函数
        void * args;            //任务函数的参数
        pthread_t tid;          //线程的id
        static int threadNum;     //当前线程的数量, 充当编号中的一个字符Ti
    private:
        //为了兼容pthread接口创造的入口函数
        //把this指针传进来, 在self->task(self->args)这一步才是真正的执行函数
        static void * start_routine(void * args) {
            Thread * self = static_cast<Thread * >(args);
            void * ret = self->task(self->args);
            return ret;
        }
    public:
        Thread() {
            char buf[num];
            snprintf(buf, sizeof buf, "T%d", threadNum++);
            name = buf;
        }
        ~Thread() {}

        void start(func_t func, void * args = nullptr) {
            this->task = func;
            this->args = args;
            int n = pthread_create(&tid, nullptr, &start_routine, static_cast<void *>(this));
            assert(n == 0);
            (void)n;
        }

        void join() {
            int n = pthread_join(tid, nullptr);
            assert(n == 0);
            (void)n;
        }

        std::string thredName() {
            return name;
        }
    };

    int Thread::threadNum = 1;

}