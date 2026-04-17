#include "Thread.hpp"
#include "LockGuard.hpp"
#include <queue>
#include <vector>
#include <mutex>
#include <pthread.h>
#include <unistd.h>

const int gNum = 3; //默认线程数量

template <class T>
class ThreadPool {
private:
    int num;                        //线程数量
    std::vector<Thread *> threads;  //线程指针数组
    std::queue<T> taskQueue         //任务队列
    pthread_cond_t cond;            //阻塞和唤醒线程的条件变量
    pthread_mutex_t mutex;          //保护任务队列的锁
    static ThreadPool<T> * tp;      //静态变量, 单例实例
    static std::mutex signleLock    //静态互斥锁, 保护单例创建
private:
    //私有构造函数, 单例设计, 禁止外部创建
    ThreadPool(const int & num = gNum) {
        this->num = num;
        //初始化条件变量和互斥锁
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
        for (int i = 0; i < this->num; i++) {
            threads.push_back(new Thread());
        }
    }

    //禁用和赋值
    void operator = (const ThrePo   )
};

template<class T>
ThreadPool<T> * ThreadPool<T>::tp = nullptr;

template<class T>
std::mutex ThreadPool<T>::signleLock;
