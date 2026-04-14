#include "Thread.hpp"

int tickets = 114514;

pthread_mutex_t mutex;

void * getTicket(void * args) {
    //上下文切走, 条件已经不满足了
    //但if已经进来了
    //原子性

    while (true) {
        pthread_mutex_lock(&mutex);
        if (tickets > 0) {
            // usleep(11451);
            std::cout << "线程" << (char *)args << "抢票成功, 票数: " << tickets << "\n";
            tickets--;
            pthread_mutex_unlock(&mutex);
        }
        else {
            pthread_mutex_unlock(&mutex);
            break;
        }
    }

    return nullptr;
}

void test() {
    //不能在每个线程内都初始化一次, 全局只能初始化一次
    pthread_mutex_init(&mutex, nullptr);

    Thread t1(&getTicket, (void *)"thread_1", 1);
    Thread t2(&getTicket, (void *)"thread_2", 2);
    Thread t3(&getTicket, (void *)"thread_3", 3);
    Thread t4(&getTicket, (void *)"thread_4", 4);


    t1.join();
    t2.join();
    t3.join();
    t4.join();

}

int main(void) {
    test();

    return 0;
}