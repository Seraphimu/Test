/**
 * 核心思想: 
 * 生产者提出问题, 消费者解答问题. 
 * 没有问题时消费者阻塞
 * 问题塞满时生产者阻塞
 */
#include <iostream>
#include <ctime>
#include <queue>
#include <algorithm>
#include <unistd.h>
#include <pthread.h>

template<typename T>
class BlockQueue {
private:
    std::queue<T> q;
    size_t capacity;
    pthread_mutex_t mutex;
    pthread_cond_t c_cond;
    pthread_cond_t p_cond;
public:
    BlockQueue(int capacity = 5) {
        this->capacity = capacity;
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&c_cond, nullptr);
        pthread_cond_init(&p_cond, nullptr);
    }
    ~BlockQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&c_cond);
        pthread_cond_destroy(&p_cond);
    }

    void productData(T data) {
        //用锁保证原子操作
        lockQueue();//P()
        //如果队列满, 进入等待状态
        while (isFull()) {
            producerWait();
        }
        //向队尾插入一个 data结构, 仅此而已
        q.push(data);
        unlockQueue();//V()
        //提出一个问题就把所有的消费者唤醒
        consumerWakeup();
    }

    void consumerData(T & data) {
        lockQueue();
        while (isEmpty()) {
            consumerWait();
        }
        //取数据, 用于运算
        data = q.front();
        q.pop();
        unlockQueue();
        //只要解决了一个问题, 就把所有的生产者线程唤醒
        producerWakeup();
    }
private:
    bool isFull() {
        return q.size() == capacity;
    }
    bool isEmpty() {
        return q.empty();
    }
    void consumerWait() {
        std::cout << "consumer wait...\n";
        pthread_cond_wait(&c_cond, &mutex);
    }
    void consumerWakeup() {
        std::cout << "wake up consumer...\n";
        pthread_cond_broadcast(&c_cond);
        // pthread_cond_signal(&c_cond);
    }

    void producerWait() {
        std::cout << "producer wait...\n";
        pthread_cond_wait(&p_cond, &mutex);
    }
    void producerWakeup() {
        std::cout << "wake up producer...\n";
        pthread_cond_broadcast(&p_cond);
        // pthread_cond_signal(&p_cond);
    }
    void lockQueue() {
        pthread_mutex_lock(&mutex);
    }
    void unlockQueue() {
        pthread_mutex_unlock(&mutex);
    }
};

class Task {
private:
    int a;
    int b;
public:
    Task(int a = 0, int b = 0) {
        this->a = a;
        this->b = b;
    }

    int run() {
        return a + b;
    }
    int getA() {
        return a;
    }
    int getB() {
        return b;
    }
};

BlockQueue<Task> * q;

void * consumer(void * args) {
    long id = (long)args;
    while (true) {
        Task t(0, 0);
        //解决一个问题, 并打印结果
        q->consumerData(t);
        std::cout << "consumer " << id << " consumes a task: " 
        << t.getA() << " + " << t.getB() << " = " << t.run() << "\n";
        std::cout << "================================\n";
        sleep(3);
    }
}

void * producer(void * args) {
    long id = (long)args;
    while (true) {
        //生成两个随机数, 做运算用.
        int x = rand() % 10 + 1;
        int y = rand() % 10 + 1;
        Task t(x, y);

        std::cout << "productor " << id << " produnces a task: " 
        << x << " + " << y << " = " << "?" << std::endl;
        //提出一个问题, 并打印结果
        q->productData(t);
        std::cout << "================================\n";
        sleep(7);
    }
}

int main(void) {
    //创建随机种子, 随机数的前置工作
    srand((size_t)time(nullptr));
    //实例化一个阻塞队列
    q = new BlockQueue<Task>;

    //维护两个线程资源
    pthread_t p, p1, p2;
    pthread_t c;
    pthread_create(&p, nullptr, &producer, (void *)0);
    pthread_create(&p1, nullptr, &producer, (void *)1);
    pthread_create(&p2, nullptr, &producer, (void *)2);
    pthread_create(&c, nullptr, &consumer, (void *)0);

    pthread_join(p, nullptr);
    pthread_join(c, nullptr);

    return 0;
}