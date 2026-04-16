/**
 * sem_wait()  decrements  (locks)  the  semaphore  pointed  to by sem.  
 * If the semaphore's value is greater than zero, then the decrement proceeds, 
 * and the function returns, immediately.
 * If  the semaphore currently has the value zero, 
 * then the call blocks until either it becomes possible 
 * to perform the decrement (i.e., the semaphore value rises above zero), 
 * or a signal handler interrupts the call.
 */

#include <iostream>
#include <vector>
#include <cassert>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

//队列默认最大容量
static const int gcap = 5;

template <typename T>
class RingQueue {
private:
    std::vector<T> q;               //用数组实现循环队列
    int cap;                        //最大容量
    sem_t spaceSem;                 //生产者信号量
    sem_t dataSem;                  //消费者信号量
    int productorStep;              //
    int consumerStep;
    pthread_mutex_t pMutex;
    pthread_mutex_t cMutex;
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
    RingQueue(const int & cap = gcap) :
    q(cap), cap(cap)
    {
        int n = sem_init(&spaceSem, 0, this->cap);
        assert(n == 0);
        n = sem_init(&dataSem, 0, 0);
        assert(n == 0);

        productorStep = consumerStep = 0;
        pthread_mutex_init(&pMutex, nullptr);
        pthread_mutex_init(&cMutex, nullptr);
    }

    ~RingQueue() {
        sem_destroy(&spaceSem);
        sem_destroy(&dataSem);

        pthread_mutex_destroy(&pMutex);
        pthread_mutex_destroy(&cMutex);
    }

    void push(const T & in) {
        P(spaceSem);
        pthread_mutex_lock(&pMutex);
        //等价于productorStep, productorStep = (p + 1) %cap
        q[productorStep++] = in;
        productorStep %= cap;
        pthread_mutex_unlock(&pMutex);
        V(dataSem);
    }
    void pop(T * out) {
        P(dataSem);
        pthread_mutex_lock(&cMutex);
        *out = q[consumerStep++];
        consumerStep %= cap;
        pthread_mutex_unlock(&cMutex);
        V(spaceSem);
    }
};