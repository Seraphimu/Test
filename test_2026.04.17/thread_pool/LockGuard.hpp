#pragma
#include <pthread.h>

/**
 * 负责加锁和解锁
 */


class Mutex {
private:
    pthread_mutex_t * pLock;
public:
    Mutex(pthread_mutex_t * pLock = nullptr) {
        this->pLock = pLock;
    }
    void lock() {
        if (pLock) pthread_mutex_lock(pLock);
    }
    void unlock() {
        if (pLock) pthread_mutex_unlock(pLock);
    }
    ~Mutex() {}
};


class LockGuard {
private:
    Mutex mutex;
public:
    LockGuard(pthread_mutex_t * mutex) :
    mutex(mutex)
    {
        this->mutex.lock();
    }

    ~LockGuard() {
        mutex.unlock(); 
    }
};

