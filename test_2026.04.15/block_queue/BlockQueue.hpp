#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

static const int gMax = 5;

template<typename T>
class BlockQueue {
private:
    std::queue<T> q;            //队列
    int maxCap;                 //最大容量模拟
    pthread_mutex_t mutex;      //互斥量
    pthread_cond_t pCond;       //生产者等待条件
    pthread_cond_t cCond;       //消费者等待条件
public:
    //重载构造函数
    // BlockQueue() {}
    BlockQueue(int maxCap = gMax) {
        this->maxCap = maxCap;
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&pCond, nullptr);
        pthread_cond_init(&cCond, nullptr);
    }
    //析构函数, 释放资源
    ~BlockQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&pCond);
        pthread_cond_destroy(&cCond);
    }

    //添加任务
    void Push(const T & task) {
        //加锁, 保证队列操作不会被其他线程干扰
        pthread_mutex_lock(&mutex);
        while (isFull()) {
            //如果队列满了, 生产者线程阻塞, 并解除mutex
            //等回来的时候, 先加锁, 再利用while循环再判断一次
            //避免线程被唤醒时isFull()条件不满足了, 有可能其他线程已经操作过了,
            //自己的上下文可能有些过时了, 需要重新判断一次.
            pthread_cond_wait(&pCond, &mutex);
        }
        q.push(task);
        pthread_mutex_unlock(&mutex);
        //如果生产好了, 就唤醒消费者
        pthread_cond_signal(&cCond);
    }

    //消费任务
    void Pop(T * task) {
        pthread_mutex_lock(&mutex);
        while (isEmpty()) {
            pthread_cond_wait(&cCond, &mutex);
        }
        *task = q.front();
        q.pop();
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&pCond);
    }
private:
    //为空时返回真
    bool isEmpty() {
        return q.empty();
    }
    //为满时返回真
    bool isFull() {
        return q.size() == maxCap;
    }
};