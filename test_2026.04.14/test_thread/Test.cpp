/**
 * 为什么线程看不到退出信号? 线程出异常, 整个进程都没了, join默认认为函数会调用成功(设计角度)
 * tid究竟是什么？
 * 每个线程都有独立栈
 * 
 * 线程分离
 * 
 * 
 * 栈在线程库??怎么理解?库?代码段, 数据段它也要有, 它也是代码和数据. 在库的地址范围内.
 * 共享区是什么? 和库有什么关系?
 * 
 * __thread线程局部存储
 */



#include "Thread.hpp"

void * thread(void * args) {
    while (true) {
        std::cout << "new thread: " << "\n";
        sleep(1);
    }

    return nullptr;
}


void test() {
    Thread pt(&thread, nullptr);
    while (true) {
        printf("main thread\n");
        sleep(1);
    }
}

int main(void) {
    test();

    return 0;
}


// void test() {
//     Thread t1(&thread, (void *)1);
//     Thread t2(&thread, (void *)2);
//     Thread t3(&thread, (void *)3);
//     //怪不得要用sleep()线程还没执行完了, 进程先TM退出了, 玩个集贸啊.
//     // // t1.show();
//     // sleep(1);
// }

//测试pthread_self()
// #include <iostream>
// #include <cassert>
// #include <pthread.h>
// #include <unistd.h>

// void * thread(void * args) {
//     printf("new thread id: %x\n", pthread_self());
//     return nullptr;
// }

// int main(void) {
//     pthread_t tid;
//     int n = pthread_create(&tid, nullptr, &thread, nullptr);
//     assert(n == 0);

//     printf("this is main thread, my new thread id: %x\n", tid);

//     return 0;
// }

// //C++多线程
// //在Linux上实现多线程, 在底层必定调用了pthread库, 不链接pthread库跑不动
// #include <iostream>
// #include <thread>
// #include <unistd.h>

// void thread() {
//     while (true) {
//         std::cout << "新线程\n";
//         sleep(1);
//     }
// }

// int main(void) {
//     std::thread t1(thread);
    
//     while (1) {
//         std::cout << "主线程\n";
//         sleep(1);
//     }

//     return 0;
// }

// //线程取消
// /**
//  * 如果一个线程是被取消的, 它的返回值是-1
//  */
// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>

// void * thread(void * args) {
//     printf("New thread is running\n");

//     return nullptr;
// }

// int main(void) {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, &thread, nullptr);
//     pthread_cancel(tid);
//     void * ret;
//     pthread_join(tid, &ret);
//     printf("exit_code: %ld\n", (long int *)ret);

//     return 0;
// }


//exit
//在栈上, 返回前释放

//线程退出信号

// #include <iostream>
// #include <vector>
// #include <pthread.h>
// #include <unistd.h>

// #define THREAD_NUM 5

// void * thread(void * args) {
//     printf("T%d is running\n", (long int)args);
//     //在堆区上完全可以, 因为堆区是整个进程共享的
//     int * a = (int *)malloc(sizeof (int) * 10);

//     //返回值可以, 计算机会拷贝返回值到寄存器, 而地址不会被拷贝??
//     //返回地址不可以, 在栈结构上的值会被释放
//     // long int a = 10;
//     // return (void*)&a;
//     return (void *)a;
// }

// int main(void) {
//     // pthread_t tid;
//     // tid = pthread_create(&tid, nullptr, &thread, (void *)"T1");

//     pthread_t tid[THREAD_NUM];
//     // std::vector<pthread_t> tid[THREAD_NUM];
//     long int i = 0;
//     for (; i < THREAD_NUM; i++) {
//         pthread_create(&tid[i], nullptr, &thread, (void *)i);
//     }

//     for (i = 0; i < THREAD_NUM; i++) {
//         void * ret = nullptr;
//         pthread_join(tid[i], &ret);
//         printf("thread: %ld, ret: %p\n", i, (int *)ret);
//         free(ret);
//     }

//     return 0;
// }