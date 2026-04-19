#pragma once

#include <pthread.h>

/**
 * 把锁(初始化之后的)传给它, 然后让它的构造函数加锁, 析构函数解锁
 */
class LockGuard {
private:
    pthread_mutex_t * mutex;
public:
    LockGuard(pthread_mutex_t * m = nullptr) {
        mutex = m;
        if (nullptr != mutex) {
            pthread_mutex_lock(mutex);
        }
    }
    ~LockGuard() {
        if (nullptr != mutex) {
            pthread_mutex_unlock(mutex);
        }
    }
};