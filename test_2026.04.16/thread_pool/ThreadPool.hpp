#include "Thread.hpp"
#include "LockGuard.hpp"

#include <vector>
#include <queue>
#include <mutex>
#include <pthread.h>

using namespace ThreadNs;
const int gnum = 3;

template <class T>
class ThreadPool;

template<class T>
class ThreadData {
public:
    ThreadPool<T> *threadPool;
    std::string name;
    ThreadData(ThreadPool<T> *tp, const std::string &n) {
        this->threadPool = tp;
        this->name = n;
    }
};

template <class T>
class ThreadPool {
private:
    int num;
    std::vector<Thread *> threads;
    std::queue<T> q;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    
    static ThreadPool<T> *tp;
    static std::mutex sigLock;
private:
    static void * handlerTask(void * args) {
        ThreadData<T> * td = static_cast<ThreadData<T> *>(args);
        while (true) {
            T t;
            {
                LockGuard lockGuard(td->threadPool->mutex());
            }
        }
        delete td;
        return nullptr;
    }
};

template <class T>
ThreadPool<T> * ThreadPool<T>::tp = nullptr;

template <class T>
std::mutex ThreadPool<T>::sigLock;