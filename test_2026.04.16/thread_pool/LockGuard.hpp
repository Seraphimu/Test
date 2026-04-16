#pragma once

#include <iostream>
#include <pthread.h>

class Mutex {
private:
    pthread_mutex_t * m_Lock;
public:
    Mutex(pthread_mutex_t * m_Lock = nullptr) 
    : m_Lock(m_Lock) {}
    void lock() {
        //如果m_Lock有效
        if (m_Lock) {
            pthread_mutex_lock(m_Lock);
        }
    }
    void unlock() {
        if (m_Lock) {
            pthread_mutex_unlock(m_Lock);
        }
    }
    ~Mutex() {}
};

class LockGuard {
private:
    Mutex mutex;
public:
//即使因为异常, 析构函数还是会被执行解锁
    LockGuard(pthread_mutex_t * mutex) {
        this->mutex = mutex;
        this->mutex.lock();
    }
    ~LockGuard() {
        this->mutex.unlock();
    }
};