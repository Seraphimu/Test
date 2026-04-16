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