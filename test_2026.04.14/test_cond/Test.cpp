#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>

int tickets = 1000;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * start_routine(void * args) {
    std::string name = static_cast<const char *>(args);
    while (true) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        std::cout << name << "->" << tickets << "\n";
        tickets--;
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

int main(void) {
    pthread_t t[5];

    for (int i = 0; i < 5; i++) {
        char buf[64];
        snprintf(buf, sizeof buf, "T%d", i);
        pthread_create(&t[i], nullptr, &start_routine, (void *)buf);
    }

    while (true) {
        sleep(1);
        pthread_cond_signal(&cond);
        // pthread_cond_broadcast(&cond);
        std::cout << "main thread wakeup one thread..." << std::endl;
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(t[i], nullptr);
    }

    return 0;
}