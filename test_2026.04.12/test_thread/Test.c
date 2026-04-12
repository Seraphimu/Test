/**
 * cond
 * 解锁互斥锁, 阻塞线程
 */

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 5

pthread_cond_t cond;
pthread_mutex_t mutex;

void * thread_1(void *args) {
    // char * id = (char *)args
    // 野指针, 用一个很小的数强转成指针, 极易导致越界.
    long id = (long)args;
    while (1) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("T%d is wakedup...\n", id);
        sleep(1);
        pthread_mutex_unlock(&mutex);
    }
}

void * thread_2(void *args) {
    long id = (long)args;
    while (1) {
        sleep(5);
        pthread_cond_signal(&cond);
        printf("T%d is wakeing up a thread...\n", id);
    }
}

int main(void) {
    pthread_t tid[MAX];
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    long int i = 0;
    for (; i < MAX - 1; i++) {
        pthread_create(&tid[i], NULL, &thread_1, (void *)i);
    }
    pthread_create(&tid[i], NULL, &thread_2, (void*)i);

    for (i = 0; i < MAX; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

// /**
//  * 线程死锁
//  * 观察: thread_1在等mutex2被释放, thread_2在等mutex1被释放
//  */

//  #include <stdio.h>
//  #include <string.h>
//  #include <pthread.h>
//  #include <unistd.h>

//  pthread_mutex_t mutex1;
//  pthread_mutex_t mutex2;

// void * thread_1(void * args) {
    
//     pthread_mutex_lock(&mutex1);
//     int cnt = 1;
//     while (1) {
//         if (cnt++ == 5) {
//             printf("T1 is requesting a lock...\n");
//             pthread_mutex_lock(&mutex2);
//         }
//         printf("T1 is running\n");
//         sleep(1);
//     }
//     pthread_mutex_unlock(&mutex1);
// }

// void * thread_2(void * args) {
//     pthread_mutex_lock(&mutex2);
//     int cnt = 1;
//     while (1) {
//         if (cnt++ == 5) {
//             printf("T2 is requesting a lock...\n");
//             pthread_mutex_lock(&mutex1);
//         }
//         printf("T2 is running\n");
//         sleep(1);
//     }
//     pthread_mutex_lock(&mutex2);
// }

// int main(void) {
//     pthread_t t1, t2;

//     //Init
//     pthread_mutex_init(&mutex1, NULL);
//     pthread_mutex_init(&mutex2, NULL);

//     //Create Threads
//     pthread_create(&t1, NULL, &thread_1, NULL);
//     pthread_create(&t2, NULL, &thread_2, NULL);

//     pthread_join(t1, NULL);
//     pthread_join(t2, NULL);

//     pthread_mutex_destroy(&mutex1);
//     pthread_mutex_destroy(&mutex2);

//     return 0;
// }


// /**
//  * mutex引入、
//  * 直观理解: 加锁期间, 一个全局变量只能在同一时刻被一个线程访问
//  *  */

//  #include <stdio.h>
//  #include <string.h>
//  #include <pthread.h>
//  #include <unistd.h>
//  #define MAX 5

//  pthread_mutex_t mutex;
//  int tickets = 100;

//  void * getTicket(void * args) {
//     long id = (long)args;
//     while (1) {
//         usleep(600000);
//         //上锁
//         pthread_mutex_lock(&mutex);
//         if (tickets > 0) {
//             //有票
//             tickets--;
//             printf("thread%ld, get a ticket, remain: %d\n", id, tickets);
//             pthread_mutex_unlock(&mutex);
//         }
//         else {
//             pthread_mutex_unlock(&mutex);
//             break;
//         }
//     }
//  }
 
//  int main(void) {
//     pthread_t tid[MAX];
//     //初始化锁
//     pthread_mutex_init(&mutex, NULL);
//     long i = 0;
//     for (; i < MAX; i++) {
//         pthread_create(&tid[i], NULL, &getTicket, (void *)i);
//     }

//     for (i = 0; i < MAX; i++) {
//         pthread_join(tid[i], NULL);
//     }
//     //销毁锁
//     pthread_mutex_destroy(&mutex);

//     return 0;
//  }



// //mutex
// /**
//  * 无法正常退出, 居然能把票买成负数!
//  */
// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>
// #define THREAD_NUM 5
// int tickets = 100;
// void * getTickets(void * args) {
//     long id = (long)args;

//     while (1) {
//         if (tickets > 0) {
//             //有票
//             usleep(500000);
//             tickets--;
//             printf("thread[%d], tid: %x get a ticket, the remain is %d\n", 
//                 (long)args, pthread_self(), tickets);
//         }
//         else {
//             //无票
//             break;
//         }
//     }
// }

// int main(void) {
//     //创建5个线程
//     pthread_t tid[5];
//     //使用long类型避免类型转换警告, int只有4字节, 而Linux下指针有8字节
//     for (long int i = 0; i < THREAD_NUM; i++) {
//         pthread_create(&tid[i], NULL, &getTickets, (void *)i);
//     }

//     //释放5个进程
//     for (long int i = 0; i < 5; i++) {
//         pthread_join(tid[i], NULL);
//     }

//     return 0;
// }


// //线程分离

// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// void * thread(void *) {
//     int cnt = 1;
//     pthread_detach(pthread_self());
//     while (1) {
//         printf("New thread, tid: %x, pid: %d\n", 
//             pthread_self(), getpid());
//         sleep(1);
//         if (cnt++ == 3) {
//             pthread_exit(NULL);
//         }
//     }
// }

// int main(void) {
//     pthread_t tid;
//     int p = pthread_create(&tid, NULL, &thread, NULL);
//     assert(p == 0);

//     //Wait the thread to detach
//     sleep(2);
//     if (pthread_join(tid, NULL) == 0) {
//         printf("Wait Success\n");
//     }
//     else {
//         printf("Wait Failed\n");
//     }

//     return 0;
// }


// // pthread_join函数

// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// long retVal = 10;

// void * thread(void * args) {
//     int cnt = 1;
//     while (1) {
//         printf("New thread, tid: %x, pid: %d\n", 
//             pthread_self(), getpid());
//         sleep(1);
//         if (cnt++ == 3) {
//             //怎么不和waitpid()一样设置成一个int类型的指针呢? 一定有其深意!
//             pthread_exit((void *)retVal);
//         }
//     }
// }

// int main(void) {
//     pthread_t tid;
//     int p = pthread_create(&tid, NULL, &thread, NULL);
//     assert(p == 0);

//     printf("线程等待中...\n");
//     void * ret = NULL;
//     pthread_join(tid, &ret);
//     printf("New thread has exited, exit code: %ld\n", (long)ret);

//     return 0;
// }









// //pthread_cancel函数：

// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// void * thread(void * args) {
//     while (1) {
//         printf("New thread, tid: %x, pid: %d\n", pthread_self(), getpid());
//         sleep(1);    
//     }
// }

// int main(void) {
//     pthread_t tid;
//     int ret = pthread_create(&tid, NULL, &thread, NULL);
//     assert(ret == 0);

//     int cnt = 1;
//     while (1) {
//         printf("Main thread, tid: %x, pid: %d\n", pthread_self(), getpid());
//         sleep(1);

//         if (cnt++ >=3) {
//             if (!pthread_cancel(tid)) {
//                 printf("线程%x, 成功退出\n", tid);
//                 break;
//             }
//         }
//     }

//     return 0;
// }


// //pthread_exit函数：

// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// void * thread(void * args) {
//     int cnt = 1;
//     while (1) {
//         printf("New Trhead, tid: %x, pid: %d\n", pthread_self(), getpid());
//         sleep(1);
//         if (cnt++ == 3) {
//             pthread_exit(NULL);
//         }
//     }
// }

// int main(void) {
//     pthread_t tid;
//     pthread_create(&tid, NULL, &thread, NULL);

//     while (1) {
//         printf("Main thread, tid: %x, pid: %d\n", pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }


// //使用return语句退出线程

// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// void * threadRunning(void * args) {
//     int cnt = 1;
//     while (cnt++ <= 5) {
//         printf("新线程: tid: %x, pid: %d\n", pthread_self(), getpid());
//         sleep(1);
//     }
//     return NULL;
// }

// int main(void) {
//     pthread_t tid;
//     int ret = pthread_create(&tid, NULL, &threadRunning, NULL);
//     assert(ret == 0);

//     while (1) {
//         printf("主线程: tid: %x, pid: %d\n", pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }


// //创建4个线程，然后打印出各自的pid和线程id

// #include <stdio.h>
// #include <assert.h>
// #include <string.h>
// #include <pthread.h>
// #include <unistd.h>

// #define MAX 5

// void * threadFunc(void * tid) {
//     while (1) {
//         printf("线程%d, 线程id: %x, 进程id: %d\n", 
//             (unsigned long)tid, pthread_self(),getpid());
//         sleep(1);
//     }
// }

// int main(void) {
//     pthread_t tids[MAX];
//     memset(tids, sizeof tids, 0);

//     for (int i = 0; i < MAX; i++) {
//         int ret = pthread_create(&tids[i], NULL, &threadFunc, (void *)i);
//         assert(ret == 0);
//     }

//     while (1) {
//         printf("主线程id: %x, 进程id: %d\n", pthread_self(), getpid());
//         sleep(1);
//     }

//     return 0;
// }



// /**
//  * 从该代码片段可知: 
//  * 线程之间共享数据段
//  */

// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// int global = 0;

// void * testThread(void * args) {
//     while (1) {
//         printf("New Thread name: %s, global = %d, &global = %p\n", 
//             (char *)args, global++, &global);
//         sleep(1);
//     }
// }

// int main(void) {
//     pthread_t tid;
//     int ret = pthread_create(&tid, NULL, testThread, (void *)"1St");
//     assert(ret == 0);
//     (void)ret;

//     while (1) {
//         printf("Main Thread! My son tid: 0x%x, global = %d, &global = %p\n", 
//             tid, global++, &global);
//         sleep(1);
//     }

//     return 0;
// }











// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// void * newFunc(void * arg) {
//     char * name = (char *)arg;
//     while (1) {
//         printf("新しいスレッドです。\n");
//         printf("\t私の名は <%s> です。\n", name);
//         sleep(2);
//         break;
//     }
// }

// int main(void) {

//     pthread_t tid;
//     int ret = pthread_create(&tid, NULL, &newFunc, (void *)"最初のスレッド");

//     while (1) {
//         printf("メインスレッドです\n");
//         printf("\t私のtidは 0x%x です\n", tid);
//         sleep(2);
//         break;
//     }

    
//     return 0;
// }


// #include <stdio.h>
// #include <assert.h>
// #include <pthread.h>
// #include <unistd.h>

// int gVal = 0;

// char * fun() {
//     return "独立メソッドです\n";
// }

// void * thread_routine(void * args) {
//     const char * name = (const char *)args;
//     while (1) {
//         fun();
//         printf("新しいスレッドです:\n");
//         printf("name: %s\n", fun());
//         printf("gVal = %d\n", gVal++);
//         sleep(5);
//     }
// }

// int main(void) {
//     pthread_t tid;
//     //pthread_create(输出型参数, 不知道, 线程执行的函数, 函数的参数)
//     int ret = pthread_create(&tid, NULL, thread_routine, (void *)"thread one");
//     assert(ret == 0);
//     (void)ret;

//     while (1) {
//         char tidBuf[64];
//         snprintf(tidBuf, sizeof tidBuf, "0x%x", tid);
//         printf("メインスレッドです, tidは: %s\n", tidBuf);
//         printf("gVal = %d\n", gVal);
//         printf("&gVal = %p\n", &gVal);
//         sleep(5);
//     }

//     return 0;
// }