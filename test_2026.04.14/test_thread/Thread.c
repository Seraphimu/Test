// 这就是你老师代码在C语言中的等价实现
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// 定义函数指针类型
typedef void* (*func_t)(void*);

// 线程上下文（对应老师的Context类）
struct ThreadContext {
    struct Thread* this_thread;  // 对应 this_
    void* user_args;             // 对应 args_
};

// 线程控制块（对应老师的Thread类）
struct Thread {
    char name[64];
    func_t user_func;     // 用户函数
    void* user_args;      // 用户参数
    pthread_t tid;        // 线程ID
};

// 静态入口函数（对应 start_routine）
void* thread_entry(void* arg) {
    struct ThreadContext* ctx = (struct ThreadContext*)arg;
    
    // 通过上下文调用真正的用户函数
    void* result = ctx->this_thread->user_func(ctx->user_args);
    
    free(ctx);  // 释放上下文
    return result;
}

// 创建线程
void create_thread(struct Thread* thread, func_t func, void* args) {
    thread->user_func = func;
    thread->user_args = args;
    
    // 创建上下文
    struct ThreadContext* ctx = malloc(sizeof(struct ThreadContext));
    ctx->this_thread = thread;
    ctx->user_args = args;
    
    // 启动线程
    pthread_create(&thread->tid, NULL, thread_entry, ctx);
}

// 示例用户函数
void* my_task(void* arg) {
    int id = *(int*)arg;
    printf("任务 %d 执行中...\n", id);
    return NULL;
}

int main() {
    struct Thread t1;
    snprintf(t1.name, sizeof(t1.name), "worker-1");
    
    int task_id = 100;
    create_thread(&t1, my_task, &task_id);
    
    pthread_join(t1.tid, NULL);
    
    return 0;
}