#pragma once
#include "LockGuard.hpp"
#include "Thread.hpp"
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <mutex>

const int gNum = 3;

template<class T>
class ThreadPool {
private:
    std::queue<T> taskQueue;
    std::vector<Thread *> threads;
    int maxThreads;
    std::string name;
    static ThreadPool * tp;                //指向线程池的指针
    static std::mutex singleLock;          //C++锁
    pthread_mutex_t mutex;                 //保护任务队列
    pthread_cond_t cond;                   //线程唤醒
    struct ThreadInfo {                    //传参用, 把线程名与参数打包传过去
        ThreadPool * pool;
        std::string name;
    };
private:
//创建一批线程
    ThreadPool(const int num = gNum) {
        this->maxThreads = num;
        //初始化锁和条件变量
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
        for (int i = 0; i < maxThreads; i++) {
            Thread * newThread = new Thread();
            threads.push_back(newThread);
            std:: cout << newThread->getThreadName() << " has been created.\n";
        }
    }

    static void * start_routine(void * args) {
        ThreadInfo * ti = static_cast<ThreadInfo *>(args);
        ThreadPool * tp = ti->pool;
        std::string name = ti->name;
        delete ti;
        while (true) {
            T t;
            {
                LockGuard lockGuard(&tp->mutex);
                while (tp->isQueueEmpty()) {
                    tp->threadWait();
                }
                t = tp->pop();
            }
            std::cout << name << " 获取任务: " << t.taskToString()
			<< ", 结果: " << t() << "\n";
        }
    }
    
public:
    static ThreadPool * getInstance() {
        if (tp == nullptr) {
            singleLock.lock();
            if (tp == nullptr) {
                tp = new ThreadPool();
            }
            singleLock.unlock();
        }
        return tp;
    }

    void run() {
        for (const auto & t : threads) {
            ThreadInfo * ti = new ThreadInfo();
            ti->name = t->getThreadName();
            ti->pool = this;
            t->start(&start_routine, static_cast<void *>(ti));
            std::cout << t->getThreadName() << " has been started\n";
        }
    }


    /**
     * 析构函数, 释放资源, 锁和线程对象
     */
    ~ThreadPool() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
        for (auto & t : threads) {
            delete t;
        }
    }
    T pop() {
        T t = taskQueue.front();
        taskQueue.pop();
        return t;
    }
    void push(const T & t) {
        LockGuard lockGuard(&mutex);
        taskQueue.push(t);
        threadSignal();
    }
private:
    bool isQueueEmpty() { return taskQueue.empty(); }
    void threadWait() { pthread_cond_wait(&this->cond, &this->mutex); }
    void threadSignal() { pthread_cond_signal(&cond); }

};

template<class T>
ThreadPool<T> * ThreadPool<T>::tp = nullptr;

template <class T>
std::mutex ThreadPool<T>::singleLock;