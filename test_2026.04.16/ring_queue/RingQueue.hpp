#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
const int MAX_BUF = 5;
template<class T>
class RingQueue {
private:
    std::vector<T> buf;
    int capacity;
    sem_t full;         //表示有几个被被放上商品
    sem_t empty;        //表示有几个空闲位置
    sem_t mutex;        //互斥量
    int in;
    int out;
private:
    void P(sem_t & sem) {
        int n = sem_wait(&sem);
        assert(n == 0);
        (void)n;
    }


    void V(sem_t & sem) {
        int n = sem_post(&sem);
        assert(n == 0);
        (void)n;
    }

public:
    //缺省值必须是常量或全局变量
    RingQueue(int cap = MAX_BUF) : buf(cap) {
        this->capacity = cap;
        in = out = 0;
        int n = sem_init(&full, 0, 0);
        n = sem_init(&empty, 0, this->capacity);
        n = sem_init(&mutex, 0, 1);
        assert(n == 0);
        (void)n;
    }


    ~RingQueue() {
        sem_destroy(&full);
        sem_destroy(&empty);
        sem_destroy(&mutex);
    }


    void produce(const T & data) {
        P(empty);
        P(mutex);
        //生产
        buf[in] = data;
        in = (in + 1) % capacity;
        V(mutex);
        V(full);
    }

    void consume(T & data) {
        P(full);
        P(mutex);
        data = buf[out];
        out = (out + 1) % capacity;
        V(mutex);
        V(empty);
    }
};

// #include "BlockQueue.tpp"