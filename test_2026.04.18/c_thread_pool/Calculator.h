#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <stdio.h>
// 计算函数实现
int calculate(int x, int y, char op) {
    switch (op) {
        case '+': return x + y;
        case '-': return x - y;
        case '*': return x * y;
        case '/': 
            if (y != 0) return x / y;
            fprintf(stderr, "除零错误\n");
            return 0;
        default:
            fprintf(stderr, "未知操作符: %c\n", op);
            return 0;
    }
}

#endif