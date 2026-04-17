#include "ThreadPool.hpp"
#include "Thread.hpp"
#include "LockGuard.hpp"

#include <iostream>
#include <unistd.h>

int tickets = 114514;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * start_routine(void * args) {
    while (true) {
        /**
         * // 假设代码
                pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 定义一个pthread_mutex_t变量
                Mutex mutex(&mutex);  // ❌ 错误！  
            变量名冲突了! 
        */
        LockGuard lockGuard(&mutex);
        Thread * t = static_cast<Thread *>(args);
        if (tickets > 0) {
            tickets--;
            // printf("线程%s成功抢票, 票数: %d\n", t->getName(), tickets);
            std::cout << "线程" << t->getName() <<"成功抢票, 剩余票数: " 
            << tickets << "\n";
            // sleep(1);
        }
        else {
            break;
        }
    }

    return nullptr;
}

int main(void) {

    Thread t[5];
    for (int i = 0; i < 5; i++) {
        t[i].start(&start_routine, static_cast<void *>(&t[i]));
    }

    for (int i = 0; i < 5; i ++) {
        t[i].join();
    }

    // Thread t;
    // t.start(&start_routine, nullptr);
    // t.join();
    // pthread_t t[5];    
    // for (long i = 0; i < 5; i++) {
    //     pthread_create(&t[i], nullptr, &start_routine, (void *)i);
    // }

    // for (long i = 0; i < 5; i++) {
    //     pthread_join(t[i], nullptr);
    // }

    return 0;
}