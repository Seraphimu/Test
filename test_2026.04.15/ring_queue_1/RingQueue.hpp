#include <iostream>
#include <vector>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

static const int gMax = 5;

template<typename T>
class RingQueue {
private:
    std::vector<T> q;           //队列
    int maxCap;                 //最大容量
    int in;                     //生产者下标
    int out;                    //消费者下标
    sem_t full;                 //缓冲满信号量
    sem_t empty;                //缓冲空信号量
    sem_t mutex;                //互斥量
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
    RingQueue(int cap = gMax) {
        //最大大小别忘了初始化
        this->maxCap = cap;
        //初始化满槽位为0, 即没有一个位置有数据
        int n = sem_init(&full, 0, 0);
        assert(n == 0);
        //初始化空糟位为队列大小, 即一个数据没有
        n = sem_init(&empty, 0, cap);
        assert(n == 0);
        //bug?
        n = sem_init(&mutex, 0, 1);
        assert(n == 0);
        (void)n;

        in = out = 0;
        //给vector预分配空间
        q.resize(cap);
    }
    ~RingQueue() {
        sem_destroy(&full);
        sem_destroy(&empty);
        sem_destroy(&mutex);
    }

    void product(const T & data) {
        //减少一个可用槽位, 如果没有可用槽位, 则阻塞等待
        //极端情况: 有maxCap个线程, 同时写满缓冲区, 拿不出空位, 后续线程就要被阻塞
        P(empty);
        P(mutex);
        q[in] = data;
        in = (in + 1) % maxCap;
        V(mutex);
        //增加一个已填充槽位
        V(full);
    }

    void consume(T & data) {
        P(full);
        P(mutex);
        data = q[out];
        out = (out + 1) % maxCap;
        V(mutex);
        V(empty);
    }
};