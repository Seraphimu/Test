#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

const int MAX_BUF = 5;

template<class T>
class BlockQueue {
private:
    std::queue<T> buf;
    int capacity;
    pthread_mutex_t mutex;
    pthread_cond_t pCond;
    pthread_cond_t cCond;
private:
    bool isFull() {
        return buf.size() == capacity;
    }
    bool isEmpty() {
        return buf.empty();
    }
public:
    BlockQueue(int cap = MAX_BUF) {
        this->capacity = cap;
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&pCond, nullptr);
        pthread_cond_init(&cCond, nullptr);
    }
    ~BlockQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&pCond);
        pthread_cond_destroy(&cCond);
    }

    void produce(const T & data) {
        pthread_mutex_lock(&mutex);
        while (isFull()) {
            //解锁并释放, 唤醒并锁定
            pthread_cond_wait(&pCond, &mutex);
        }
        //生产
        buf.push(data);
        pthread_mutex_unlock(&mutex);
        //每生产一个产品, 就唤醒一个消费者线程
        pthread_cond_signal(&cCond);
    }
    void consume(T & data) {
        pthread_mutex_lock(&mutex);
        while (isEmpty()) {
            pthread_cond_wait(&cCond, &mutex);
        }
        data = buf.front();
        buf.pop();
        pthread_mutex_unlock(&mutex);
        //每消费一个产品, 就唤醒一个生产者进程
        pthread_cond_signal(&pCond);
    }
};