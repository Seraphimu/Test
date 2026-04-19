#include "ThreadPool.hpp"
#include "Task.hpp"
#include <memory>
#include <ctime>
#include <unistd.h>

int main()
{
    srand((size_t)time(nullptr));
    //创建一个线程池, 全局唯一的实例, 并调用其run()函数
    ThreadPool<Task>::getInstance()->run();

    int x, y;
    char op;
    while(true)
    {
        x = rand() % 10 + 1;
        y = rand() % 10 + 1;
        op = opers[rand() % opers.length()];
        Task t(x, y, op, &myMath);
        ThreadPool<Task>::getInstance()->push(t);
        sleep(1);
    }

	return 0;
}




// #include "LockGuard.hpp"
// #include "Thread.hpp"
// #include "ThreadPool.hpp"
// #include "Task.hpp"
// #include <iostream>
// #include <queue>
// #include <ctime>
// #include <unistd.h>

// /**
//  * 测试Task类
//  */



// /**
//  * 先创建一批线程, 任务来了再唤醒, 没有任务就休眠
//  */
// int main(void) {


// 	return 0;
// }


//单个Task, 如果是一堆Task呢?

// int main(void) {
// 	srand((size_t)time(nullptr));
// 	while (true) {
		// int x = rand() % 100 + 1;
		// int y = rand() % 100 + 1;
		// char op = opers[rand() % opers.length()];
		// Task t(x, y, op, &myMath);
// 		std::cout << t.taskToString() << '\n';
// 		std::cout << t() << '\n';
// 		sleep(1);
// 	}

// 	return 0;
// }








// /**
//  * 使用封装的LockGuard类测试抢票
//  * 现在再加上封装的线程类
//  */
// int tickets = 114514;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// void * start_routine(void * args) {
// 	long int id = (long)args;
// 	while (true) {
// 		{
// 			LockGuard lockGuard(&mutex);
// 			if (tickets > 0) {
// 				tickets--;
// 				printf("T%ld, 抢票成功, 剩余票数: %d\n", id, tickets);
// 			}
// 			else {
// 				break;
// 			}
// 			// usleep(1000);
// 		}

// 	}

// 	return nullptr;
// }

// int main(void) {

// 	const int MAX = 5;
// 	Thread t[MAX];
// 	for (long int i = 0; i < MAX; i++) {
// 		t[i].start(&start_routine, (void *)i);
// 	}

// 	for (int i = 0; i < MAX; i++) {
// 		t[i].join();
// 	}
// 	// pthread_t t[MAX];

// 	// for (long i = 0; i < MAX; i++) {
// 	// 	pthread_create(&t[i], nullptr, &start_routine, (void *)i);
// 	// }
	
// 	// for (int i = 0; i < MAX; i++) {
// 	// 	pthread_join(t[i], nullptr);
// 	// }
	
// 	return 0;
// }
