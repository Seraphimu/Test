#pragma once
#include <string>
#include <functional>
#include <cassert>
#include <pthread.h>

class Thread {
private:
    using func_t = std::function<void * (void *)>;
    const int BUF_MAX = 64;
private:
    pthread_t tid;
    std::string name;
    static int threadNum;
    func_t task;
    void * args;
public:
    Thread() {
        /**
        1. 创建局部字符数组 buf（在栈上）
        2. 格式化字符串到 buf
        3. 将 buf 赋值给 std::string
        4. 构造函数结束，buf 被自动释放
        5. 但 name 已经拥有了字符串的独立副本！ 
        字符串: C->C++直接赋值/构造函数, 反之c_str()函数, 返回const char*
        */
        //构造线程名称
        char buf[BUF_MAX];
        snprintf(buf, sizeof buf, "T%d", threadNum++);
        // 这里是深拷贝！
        this->name = buf;
    }
    ~Thread() {    }

    //创建线程
    /**
     * 创建线程时, 把this指针传入到start_routine()中, 
     * 这个指针包含了任务函数指针, 参数等重要信息
     */
    void start(func_t task, void * args) {
        this->task = task;
        this->args = args;
        int n = pthread_create(&tid, nullptr, &start_routine, (void *)this);
        assert(n == 0);
        (void)n;

    }
    void join() {
        pthread_join(tid, nullptr);
    }
    std::string getThreadName() {
        return this->name;
    }
private:
/**
 * 接收到this指针, 并使用this->task(this->args) 执行真任务函数
 */
    static void * start_routine(void * args) {
        Thread * pt = static_cast<Thread *>(args);
        void * ret = pt->task(pt->args);
        return ret;
    }

};

int Thread::threadNum = 0;