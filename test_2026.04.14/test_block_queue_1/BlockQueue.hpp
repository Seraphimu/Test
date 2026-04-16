#pragma once

#include <iostream>
#include <queue>
#include <algorithm>
#include <ctime>
#include <pthread.h>
#include <unistd.h>

template <typename T>
class BlockQueue {
private:
    std::queue<T> q;
    int capacity;
    pthread_mutex_t mutex;
    pthread_cond_t p_cond;
    pthread_cond_t c_cond;
public:
    BlockQueue() {
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&p_cond, nullptr);
        pthread_cond_init(&c_cond, nullptr);
    }
    ~BlockQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&p_cond);
        pthread_cond_destroy(&c_cond);
    }
    void productData(T data) {
        lockQueue();
        while (isFull()) {
            producerWait();
        }
        q.push(data);
        unlockQueue();
        consumerWakeup();
    }
    void consumeData(T & data) {
        lockQueue();
        while (isEmpty()) {
            consumerWait();
        }
        data = q.front();
        q.pop();
        unlockQueue();
        producerWakeup();
    }
private:
    bool isEmpty() {
        return q.empty();
    }
    bool isFull() {
        return q.size() == capacity;
    }

    void lockQueue() {
        pthread_mutex_lock(&mutex);
    }
    void unlockQueue() {
        pthread_mutex_unlock(&mutex);
    }

    void consumerWait() {
        cout << "consumer wait\n";
        pthread_cond_wait(&c_cond, &mutex);
    }
    void consumerWakeup() {
        cout << "consumer be waked up\n";
        pthread_cond_broadcast(&c_cond);
    }
    void producerWait() {
        cout << "producer wait\n";
        pthread_cond_wait(&p_cond, &mutex);
    }
    void producerWakeup() {
        cout << "producer be waked up\n";
        pthread_cond_broadcast(&p_cond);
    }
};