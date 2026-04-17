#include "Thread.hpp"
#include "LockGuard.hpp"
#include <queue>
#include <vector>
#include <mutex>
#include <pthread.h>
#include <unistd.h>

const int gNum = 3;

template <class T>
class ThreadData {
public:
//这里编译可能会报错, 因为我没有在这个类之前声明ThreadPool类
    ThreadPool<T> * threaadPool;    //指向线程池的指针
    std::string name;               //线程名称
public:
    ThreadData(ThreadPool<T> * tp, const std::string & n) : threaadPool(tp), name(n){ }
};

template <class T>
class ThreadPool {
private:
    int num;                        //线程数量
    std::vector<Thread *> threads          //存放线程池内的线程指针
    std::queue<T> taskQueue         //任务队列
    pthread_mutex_t mutex;          //保护任务队列的互斥锁
    pthread_cond_t cond;            //条件变量, 用于线程阻塞和唤醒
    static ThreadPool<T> *tp;       //静态成员, 单例实例
    static std::mutex singleLock;   //静态互斥锁, 保护单例
private:
    //构造函数, 向线程池中主动创建gNum个线程, 并初始化锁和条件变量
    ThreadPool(const int & num = gNum) : num(num) {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
        for (int i = 0; i < num; i++) {
            //thread是什么类型的? T类型的, 相当于threads[i] = new Thread()
            threads.push_back(new Thread());
        }
    }
    static void * handlerTask(void * args) {
        ThreadData<T> * td = static_cast<ThreadData<T> *>(args);
        while (true) {
            T t;
            LockGuard lockGuard(td->threaadPool->mutex);
        }
    }
    //待解决问题, 这TM什么鬼
    // 线程池是单例模式 + 资源管理
    // 1. 只能有一个实例（单例）
    // 2. 管理线程和共享队列（资源）
    // 3. 拷贝会导致多个实例共享资源
    // 4. 析构时会导致重复释放
    //防止等于号赋值
    void operator=(const ThreadPool &) = delete;
    //防止构造时拷贝
    ThreadPool(const ThreadPool &) = delete;
public:
//锁和条件变量操作, 这部分不是应该直接用封装好的Mutex类吗? 先写着
    // 加锁队列
    void lockQueue() { pthread_mutex_lock(&_mutex); }
    // 解锁队列
    void unlockQueue() { pthread_mutex_unlock(&_mutex); }    
    // 检查队列是否为空
    bool isQueueEmpty() { return _task_queue.empty(); }

    // 线程等待条件变量
    void threadWait() { pthread_cond_wait(&_cond, &_mutex); }
    T pop() {
        T t = taskQueue.front();
        taskQueue.pop();
        return t;
    }

    //它的作用是什么? 返回一个本线程池的锁, 还是控制队列的给谁用?
    pthread_mutex_t * mutex() {
        return &this->mutex;
    }
public:
    //启动所有线程
    void run() {
        for (const auto & t : threads) {
            //为每个线程创建ThreadData对象
            ThreadData<T> * td = new ThreadData<T>(this, t->getName());
            //启动线程
            // t->start();
        }
    }
};


template <class T>
ThreadPool<T> * ThreadPool<T>::tp = nullptr;

template <class T>
std::mutex ThreadPool<T>::singleLock;
