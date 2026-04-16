#include "RingQueue.hpp"
RingQueue<int> * q;

void * consumer(void * args) {
    printf("消费者线程启动成功\n");
    long id = (long)args;
    while (true) {
        int ret;
        q->consume(ret);
        printf("消费者%ld: %d has been poped!\n", args, ret);
        sleep(5);
    }
}

void * producer(void * args) {
    printf("生产者线程启动成功\n");
    long id = (long)args;
    while (true) {
        int r = rand() % 1145;
        q->product(r);
        printf("生产者%ld: %d has been pushed!\n",args, r);
        sleep(5);
    }
}

int main(void) {
    srand((unsigned int)time(nullptr));
    q = new RingQueue<int>;
    const int MAX = 2;
    pthread_t p[MAX];
    pthread_t c[MAX];
    for (long i = 0; i < MAX; i++) {
        pthread_create(&p[i], nullptr, &consumer, (void *)i);
        pthread_create(&c[i], nullptr, &producer, (void *)i);
    }

    for (long i = 0; i < MAX; i++) {
        pthread_join(p[i], nullptr);
        pthread_join(c[i], nullptr);
    }
    
    return 0;
}