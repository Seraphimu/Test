#pragma once

#include <iostream>
#include <string>
#include <functional>

class Task {
private:
    int x;
    int y;
    char op;
    using func_t = std::function<int(int, int, char)>;
    func_t callBack;
public:
    Task() {};
    Task(int x, int y, char op, func_t f) : 
    x(x), y(y), op(op), callBack(f){};
    /**
     * 重载()运算符, 可以直接对象名t()执行. 其中第二个括号里放的是函数的参数, 这里为空
     */
    std::string operator()() {
        int result = callBack(x, y, op);
        char buffer[1024];
        snprintf(buffer, sizeof buffer, "%d %c %d = %d", x, op, y, result);
        return buffer;
    }
    std::string taskToString() {
        char buffer[1024];
        snprintf(buffer, sizeof buffer, "%d %c %d = ?", x, op, y);
        return buffer;
    }
};

const std::string oper = "+-*/%";

int myMath(int x, int y, char op) {
    int ret = 0;
    switch (op) {
    case '+':
        ret = x + y;
        break;
    case '-':
        ret = x - y;
        break;
    case '*':
        ret = x * y;
        break;
    case '/':
        if (y == 0) {
            perror("除0错误");
            ret = -1;
        }
        else {
            ret = x / y;
        }
        break;
    case '%':
        if (y == 0) {
            perror("模0错误");
            ret = -1;
        }
        else {
            ret = x % y;
        }
        break;
    default:
        break;
    }

    return ret;
}