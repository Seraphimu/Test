#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define MAX 5
void * task(void * args) {
    while (1) {
        printf("这是一个新线程: %ld\n", (long)args);
        sleep(1);
    }
}

int main(void) {
    pthread_t tid[MAX];
    for (long i = 0; i < MAX; i++) {
        pthread_create(&tid[i], NULL, &task, (void *)i);
        printf("线程%d已创建\n", tid[i]);
    }
    while (1) {
        printf("这是主线程\n");
        sleep(1);
    }    

    return 0;
}