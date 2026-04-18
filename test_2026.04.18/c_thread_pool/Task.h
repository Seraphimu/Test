#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 任务结构体 - 代替C++的Task类
typedef struct {
    int x;
    int y;
    char op;
    int (*callback)(int, int, char);  // 函数指针代替std::function
} Task;

// 任务执行函数
typedef int (*TaskFunc)(int, int, char);

// 创建任务
Task* create_task(int x, int y, char op, TaskFunc func) {
    Task* task = (Task*)malloc(sizeof(Task));
    if (task) {
        task->x = x;
        task->y = y;
        task->op = op;
        task->callback = func;
    }
    return task;
}

// 执行任务
void execute_task(Task* task) {
    if (task && task->callback) {
        int result = task->callback(task->x, task->y, task->op);
        printf("%d %c %d = %d\n", task->x, task->op, task->y, result);
    }
}

// 销毁任务
void destroy_task(Task* task) {
    if (task) {
        free(task);
    }
}

#endif