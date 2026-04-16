#include "RingQueue.hpp"

RingQueue<int> q;

void * producer(void * args) {
    //用64位的整数与指针完成类型转换就不用担心精度丢失了.
    u_int64_t id = (u_int64_t)args;
    while (true) {
        int data = rand() % 114;
        q.produce(data);
        printf("生产者[%d]放入: %d\n", id, data);
        sleep(1);
    }
}

void * consumer(void * args) {
    u_int64_t id = (u_int64_t)args;
    while (true) {
        int data;
        q.consume(data);
        printf("消费者[%d]取出: %d\n", id, data);
    }
}

int main(void) {
    srand((unsigned int)time(nullptr));
    const int MAX = 2;
    pthread_t p[MAX];
    pthread_t c[MAX];
    for (u_int64_t i = 0; i < MAX; i++) {
        pthread_create(&p[i], nullptr, &producer, (void *)i);
        pthread_create(&c[i], nullptr, &consumer, (void *)i);
    }

    for (int i = 0; i < MAX; i++) {
        pthread_join(p[i], nullptr);
        pthread_join(c[i], nullptr);
    }

    return 0;
}