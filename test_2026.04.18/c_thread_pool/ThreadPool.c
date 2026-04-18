#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 包含任务定义
#include "Task.h"
#include "Calculator.h"

// ============================
// 线程池数据结构定义
// ============================

// 任务队列节点
typedef struct TaskNode {
    Task* task;                // 任务指针
    struct TaskNode* next;     // 下一个节点
} TaskNode;

// 任务队列
typedef struct {
    TaskNode* head;           // 队头
    TaskNode* tail;           // 队尾
    int size;                 // 队列大小
} TaskQueue;

// 工作线程结构
typedef struct {
    pthread_t thread_id;      // 线程ID
    int thread_num;           // 线程编号
    int is_running;           // 是否运行
} WorkerThread;

// 线程池主结构
typedef struct {
    WorkerThread* workers;    // 工作线程数组
    int worker_count;         // 线程数量
    
    TaskQueue* task_queue;    // 任务队列
    pthread_mutex_t queue_mutex;  // 队列互斥锁
    pthread_cond_t queue_cond;    // 队列条件变量
    
    int shutdown;             // 关闭标志
} ThreadPool;

// ============================
// 任务队列操作
// ============================

// 初始化任务队列
TaskQueue* create_task_queue() {
    TaskQueue* queue = (TaskQueue*)malloc(sizeof(TaskQueue));
    if (queue) {
        queue->head = NULL;
        queue->tail = NULL;
        queue->size = 0;
    }
    return queue;
}

// 添加任务到队列
void enqueue_task(TaskQueue* queue, Task* task) {
    TaskNode* new_node = (TaskNode*)malloc(sizeof(TaskNode));
    if (!new_node) return;
    
    new_node->task = task;
    new_node->next = NULL;
    
    if (queue->tail) {
        queue->tail->next = new_node;
        queue->tail = new_node;
    } else {
        queue->head = queue->tail = new_node;
    }
    queue->size++;
}

// 从队列取出任务
Task* dequeue_task(TaskQueue* queue) {
    if (!queue->head) return NULL;
    
    TaskNode* node = queue->head;
    Task* task = node->task;
    
    queue->head = node->next;
    if (!queue->head) {
        queue->tail = NULL;
    }
    
    free(node);
    queue->size--;
    return task;
}

// 检查队列是否为空
int is_queue_empty(TaskQueue* queue) {
    return queue->size == 0;
}

// 销毁任务队列
void destroy_task_queue(TaskQueue* queue) {
    TaskNode* current = queue->head;
    while (current) {
        TaskNode* next = current->next;
        if (current->task) {
            destroy_task(current->task);
        }
        free(current);
        current = next;
    }
    free(queue);
}

// ============================
// 工作线程函数
// ============================

// 工作线程的入口函数
void* worker_function(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    
    printf("工作线程启动\n");
    
    while (1) {
        pthread_mutex_lock(&pool->queue_mutex);
        
        // 等待任务（条件变量等待）
        while (is_queue_empty(pool->task_queue) && !pool->shutdown) {
            pthread_cond_wait(&pool->queue_cond, &pool->queue_mutex);
        }
        
        // 检查是否需要关闭
        if (pool->shutdown && is_queue_empty(pool->task_queue)) {
            pthread_mutex_unlock(&pool->queue_mutex);
            break;
        }
        
        // 获取任务
        Task* task = dequeue_task(pool->task_queue);
        pthread_mutex_unlock(&pool->queue_mutex);
        
        if (task) {
            printf("线程处理任务: ");
            execute_task(task);
            destroy_task(task);
        }
    }
    
    printf("工作线程结束\n");
    return NULL;
}

// ============================
// 线程池操作
// ============================

// 创建线程池
ThreadPool* create_thread_pool(int worker_count) {
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    if (!pool) return NULL;
    
    // 初始化互斥锁和条件变量
    pthread_mutex_init(&pool->queue_mutex, NULL);
    pthread_cond_init(&pool->queue_cond, NULL);
    
    // 创建任务队列
    pool->task_queue = create_task_queue();
    pool->shutdown = 0;
    pool->worker_count = worker_count;
    
    // 创建工作线程
    pool->workers = (WorkerThread*)malloc(sizeof(WorkerThread) * worker_count);
    for (int i = 0; i < worker_count; i++) {
        pool->workers[i].thread_num = i;
        pool->workers[i].is_running = 1;
        
        pthread_create(&pool->workers[i].thread_id, NULL, 
                      worker_function, pool);
    }
    
    printf("线程池创建成功，有 %d 个工作线程\n", worker_count);
    return pool;
}

// 向线程池添加任务
void add_task_to_pool(ThreadPool* pool, Task* task) {
    pthread_mutex_lock(&pool->queue_mutex);
    enqueue_task(pool->task_queue, task);
    pthread_mutex_unlock(&pool->queue_mutex);
    
    // 通知一个等待的线程
    pthread_cond_signal(&pool->queue_cond);
}

// 销毁线程池
void destroy_thread_pool(ThreadPool* pool) {
    if (!pool) return;
    
    // 设置关闭标志
    pool->shutdown = 1;
    
    // 唤醒所有等待的线程
    pthread_cond_broadcast(&pool->queue_cond);
    
    // 等待所有线程结束
    for (int i = 0; i < pool->worker_count; i++) {
        pthread_join(pool->workers[i].thread_id, NULL);
    }
    
    // 清理资源
    pthread_mutex_destroy(&pool->queue_mutex);
    pthread_cond_destroy(&pool->queue_cond);
    
    destroy_task_queue(pool->task_queue);
    free(pool->workers);
    free(pool);
    
    printf("线程池已销毁\n");
}

// ============================
// 主程序
// ============================

int main() {
    printf("=== 纯C线程池演示 ===\n");
    
    // 创建线程池（3个工作线程）
    ThreadPool* pool = create_thread_pool(3);
    
    // 添加10个任务
    for (int i = 0; i < 10; i++) {
        int x = rand() % 100;
        int y = rand() % 20 + 1;  // 避免除零
        char ops[] = "+-*/";
        char op = ops[rand() % 4];
        
        Task* task = create_task(x, y, op, calculate);
        add_task_to_pool(pool, task);
        
        printf("添加任务: %d %c %d\n", x, op, y);
        // usleep(100000);  // 100ms间隔
        sleep(1);
    }
    
    // 等待任务完成
    printf("等待任务完成...\n");
    sleep(3);
    
    // 销毁线程池
    destroy_thread_pool(pool);
    
    return 0;
}