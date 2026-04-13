

// /**
//  * 为什么要在cond_wait()的同时释放互斥信号量mutex?
//  * 因为先互斥再等条件, 这和之前用PV写的不一样, 以前写的PV操作是先判断条件是否满足,
//  * 满足之后再进行生产/消费, 而这里是用了先互斥, cont_wait函数的设计直接避免了
//  * 先互斥, 但因为条件不满足而引发阻塞, 导致另一个进程无法访问临界资源, 导致一直死锁
//  * 
//  * 而对于cont_wait()/cont_signal()的理解可以是这样
//  * cont_wait(阻塞原因, 要解开的锁)
//  * cont_signal(阻塞原因);//通知因为这个原因而阻塞的进程可以进行工作了.
//  * 但为什么要用while循环来等, 而不用if呢?
//  * 
//  *     // pthread_cond_wait 做三件事（原子操作）：
//     // 1. 解锁mutex（让其他线程可以操作）
//     // 2. 线程进入等待状态（不占CPU）
//     // 3. 被唤醒后，重新加锁mutex
//  */
// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <time.h>

// #define QUEUE_SIZE 5
// #define MAX_ITEMS 20  // 总共生产和消费的商品数量

// // 全局共享队列
// int queue[QUEUE_SIZE];
// int front = 0;  // 队列头
// int rear = 0;   // 队列尾
// int count = 0;  // 当前队列中的商品数量

// // 同步原语
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;  // 队列不空条件
// pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;   // 队列不满条件

// // 商品计数器
// int produced_count = 0;
// int consumed_count = 0;

// // 可视化打印队列
// void print_queue(const char* tag, int thread_id, int item, int is_produce) {
//     printf("%s T%d: ", tag, thread_id);
    
//     if (is_produce) {
//         printf("生产商品 %d | ", item);
//     } else {
//         printf("消费商品 %d | ", item);
//     }
    
//     printf("队列状态: [");
//     for (int i = 0; i < QUEUE_SIZE; i++) {
//         if (i < count) {
//             printf("■");  // 有商品
//         } else {
//             printf("□");  // 空位
//         }
//     }
//     printf("] 商品数: %d/5\n", count);
// }

// // 生产者线程
// void* producer(void* arg) {
//     int thread_id = *(int*)arg;
//     int item = 1;
    
//     while (produced_count < MAX_ITEMS) {
//         pthread_mutex_lock(&mutex);
        
//         // 等待队列不满
//         //队列满了就阻塞生产者进程
//         while (count == QUEUE_SIZE) {
//             printf("生产者 T%d: 队列已满，等待中...\n", thread_id);
//             pthread_cond_wait(&not_full, &mutex);
//         }
        
//         // 生产商品
//         queue[rear] = item;
//         rear = (rear + 1) % QUEUE_SIZE;
//         count++;
//         produced_count++;
        
//         print_queue("生产者", thread_id, item, 1);
//         item++;
        
//         // 通知消费者队列不空了
//         pthread_cond_signal(&not_empty);
//         pthread_mutex_unlock(&mutex);
        
//         // 模拟生产耗时
//         sleep(1);
//     }
    
//     return NULL;
// }

// // 消费者线程
// void* consumer(void* arg) {
//     int thread_id = *(int*)arg;
    
//     while (consumed_count < MAX_ITEMS) {
//         pthread_mutex_lock(&mutex);
        
//         // 等待队列不空
//         while (count == 0) {
//             printf("消费者 T%d: 队列为空，等待中...\n", thread_id);
//             pthread_cond_wait(&not_empty, &mutex);
//         }
        
//         // 消费商品
//         int item = queue[front];
//         front = (front + 1) % QUEUE_SIZE;
//         count--;
//         consumed_count++;
        
//         print_queue("消费者", thread_id, item, 0);
        
//         // 通知生产者队列不满了
//         pthread_cond_signal(&not_full);
//         pthread_mutex_unlock(&mutex);
        
//         // 模拟消费耗时
//         sleep(2);
//     }
    
//     return NULL;
// }

// int main() {
//     pthread_t prod_thread, cons_thread;
//     int prod_id = 1, cons_id = 1;
    
//     printf("=== 生产者-消费者模型演示 ===\n");
//     printf("队列容量: 5\n");
//     printf("生产者: 每秒生产1个商品\n");
//     printf("消费者: 每2秒消费1个商品\n");
//     printf("■ = 有商品, □ = 空位\n\n");
    
//     // 创建线程
//     pthread_create(&prod_thread, NULL, producer, &prod_id);
//     pthread_create(&cons_thread, NULL, consumer, &cons_id);
    
//     // 等待线程结束
//     pthread_join(prod_thread, NULL);
//     pthread_join(cons_thread, NULL);
    
//     printf("\n=== 程序结束 ===\n");
//     printf("总共生产: %d 个商品\n", produced_count);
//     printf("总共消费: %d 个商品\n", consumed_count);
    
//     // 清理
//     pthread_mutex_destroy(&mutex);
//     pthread_cond_destroy(&not_empty);
//     pthread_cond_destroy(&not_full);
    
//     return 0;
// }


// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <time.h>
// #include <unistd.h>

// #define QUEUE_SIZE 5
// #define MAX_ITEMS 20

// int queue[QUEUE_SIZE];
// int front = 0;
// int rear = 0;
// int count = 0;

// pthread_mutex_t mutex;
// pthread_cond_t not_empty;
// pthread_cond_t not_full;

// int produced_count = 0;
// int consumed_count = 0;

// void printf_queue(const char * tag, int thread_id, int item, int is_produce)    {
//     printf("%s T%d: ", tag, thread_id);

//     if (is_produce) {
//         printf("生产商品: %d | ", item);
//     }
//     else {
//         printf("消费商品: %d | ", item);
//     }

//     printf("队列状态: [");
//     for (int i = 0; i < QUEUE_SIZE; i++) {
//         if (i < count) {
//             printf("■");  // 有商品
//         }
//         else {
//             printf("□");  // 空位
//         }
//         printf("] 商品数: %d/5\n", count);
//     }
// }

// void * consumer(void * arg) {
//     int thred_id = *(int *)arg;

//     while (consumed_count < MAX_ITEMS) {
//         pthread_mutex_lock(&mutex);

//         while (count == 0) {
//             printf("消费者 T%d: 队列为空, 等待中..\n");
//             pthread_cond_wait(&not_empty, &mutex);
//         }
        
//         //消费商品
//         int item = queue[front];
//         front = (front + 1) % QUEUE_SIZE;
//         count--;
//         consumed_count++;
//         printf_queue("消费者", thred_id, item, 0);
        
//         sleep(2);
//     }
//     return NULL;
// }

// void * producer(void * arg) {
//     int thread_id = *(int *)arg;
//     int item = 1;

//     while (produced_count < MAX_ITEMS) {
//         pthread_mutex_lock(&mutex);
//         //等待队列不满
//         while (count == QUEUE_SIZE) {
//             printf("生产者: T%d: 队列已满, 等待中...\n", thread_id);
//             pthread_cond_wait(&not_full, &mutex);
//         }
//         //若队列不满
//         queue[rear] = item;
//         rear = (rear + 1) % QUEUE_SIZE;
//         count++;
//         produced_count++;

//         printf_queue("生产者", thread_id, item, 1);
//         item++;

//         //通知消费者队列不为空
//         pthread_cond_signal(&not_empty);
//         pthread_mutex_unlock(&mutex);

//         sleep(1);
//     }

//     return NULL;
// }

// int main(void) {
//     pthread_t prod_thread, cons_thread;
//     int prod_id = 1, cons_id = 1;

//     printf("=== 生产者-消费者模型演示 ===\n");
//     printf("队列容量: 5\n");
//     printf("生产者: 每秒生产1个商品\n");
//     printf("消费者: 每2秒消费1个商品\n");
//     printf("■ = 有商品, □ = 空位\n\n");

//     pthread_create(&prod_thread, NULL, &producer, &prod_id);
//     pthread_create(&cons_thread, NULL, &consumer, &cons_id);

//     pthread_join(prod_thread, NULL);
//     pthread_join(cons_thread, NULL);

//     printf("\n=== 程序结束 ===\n");
//     printf("总共生产: %d 个商品\n", produced_count);
//     printf("总共消费: %d 个商品\n", consumed_count);
    
//     // 清理
//     pthread_mutex_destroy(&mutex);
//     pthread_cond_destroy(&not_empty);
//     pthread_cond_destroy(&not_full);

//     return 0;
// }

// /**
//  * 简单版的生/消模型
//  * 当buf为空时, Consumer被阻塞
//  * 当buf为满时, Producer被阻塞
//  * 下标起始值为0, 判断buf满, i=127, 空, i = 0
//  * 当C进程操作时, P不能介入, 反之亦然
//  */

// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// #define MAX 128

// pthread_mutex_t mutex;
// pthread_cond_t cond;

// char buf[MAX];
// int i = 0;

// void * threadConsumer(char * arg) {
//     while (1) {
//         printf("%s: \n", (char *)arg);
//         usleep(60000);
//         pthread_mutex_lock(&mutex);

//         pthread_mutex_unlock(&mutex);
//     }
// }

// void * threadProducer(char * arg) {
//     while (1) {
//         printf("%s: \n", (char *)arg);
//         usleep(60000);
//     }
// }

// int main(void) {
//     pthread_t consumer, producer;
//     pthread_mutex_init(&mutex, NULL);
//     pthread_cond_init(&cond, NULL);

//     int p1 = pthread_create(&consumer, NULL, &threadConsumer, (void *)"consumer_1");
//     int p2 = pthread_create(&producer, NULL, &threadProducer, (void *)"producer_1");
//     assert(p1 == 0 && p2 == 0);

//     while (1) {
//         printf("此时缓冲区有%d个商品\n", i);
//         sleep(2);
//     }

//     return 0;
// }

// /**
//  * 看明白了, 什么线程, 说到底还是进程代码段, 数据段的一部分罢了.
//  * 执行流是进程的一部分代码段, 但线程可以与进程共享同一个数据段, 
//  * 即可以同时访问到同一个全局变量(如果不做任何保护的话)
//  * 而所谓的多线程, 就是让进程中指定的一个函数执行流, 与主函数并发执行,
//  * 仅此而已
//  */

// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// int cnt = 10;

// void * newStream(void * arg) {
//     while (1) {
//         if (cnt > 0) {
//             printf("2. New Thread Get a Resource!, Remain: %d\n", cnt);
//             cnt--;
//             usleep(600000);
//         }
//         else {
//             printf("2. Dont have any resource: %d\n", cnt);
//             break;
//         }
//     }
// }

// int main(void) {
//     pthread_t tid;
//     pthread_t t2;
//     int p = pthread_create(&tid, NULL, &newStream, NULL);
//     int p2 = pthread_create(&t2, NULL, &newStream, NULL);
//     assert(p == 0);
//     assert(p2 == 0);

//     while (1) {
//         if (cnt > 0) {
//             printf("1. Main Thread Get a Resource!, Remain: %d\n", cnt);
//             cnt--;
//             usleep(600000);
//         }
//         else {
//             printf("1. Dont have any resource: %d\n", cnt);
//             break;
//         }
//     }

//     return 0;
// }


// /**
//  * 线程之间似乎并不共享栈空间, 比如函数参数, 局部变量这些存放在栈空间的内容并不做修改,
//  * 反而对全局变量十分甚至是九分感兴趣.
//  */
// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// void * newStream(void * arg) {
//     while (1) {
//         printf("New Stream\n");
//         sleep(2);
//     }
// }

// int main(void) {
//     pthread_t tid;
//     int ret = pthread_create(&tid, NULL, &newStream, NULL);
//     assert(ret == 0);

//     while (1) {
//         printf("Main Stream\n");
//         sleep(2);
//     }

//     return 0;
// }