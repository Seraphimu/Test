#pragma once
#include "Task.hpp"
#include <queue>
#include <pthread.h>

#define DEFAULT_MAX_THREAD 5


class ThreadPool {
private:
    std::queue<Task *> taskQueue;
    int maxThread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
public:
    ThreadPool(int max = DEFAULT_MAX_THREAD) {
        this->maxThread = max;
    }
    ~ThreadPool() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }
public:
    void getTask(Task * t) {
        t = taskQueue.front();
    }
    void putTask(Task * t) {
        lockQueue();
        taskQueue.push(t);
        unlockQueue();
        threadWakeup();
        t->run();
    }
    void threadPoolInit() {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
        pthread_t t[maxThread];
        for (int i = 0; i < maxThread; i++) {
            pthread_create(&t[i], nullptr, &start_routine, this);
        }
    }
public:
    void lockQueue() { pthread_mutex_lock(&mutex); }
    void unlockQueue() { pthread_mutex_unlock(&mutex); }
    void threadWait() { pthread_cond_wait(&cond, &mutex); }
    void threadWakeup() { pthread_cond_signal(&cond); }
private:
    bool isEmpty() { return taskQueue.empty(); }
    static void * start_routine(void * args) {
        pthread_detach(pthread_self());
        ThreadPool * tp = static_cast<ThreadPool *>(args);
        while (true) {
            tp->lockQueue();
            while (tp->isEmpty()) {
                tp->threadWait();
            }
        }
        Task * t;
        tp->getTask(t);
        tp->unlockQueue();
        t->run();
        delete[] t;
    }

};