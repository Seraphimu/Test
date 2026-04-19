
#pragma once
#include <iostream>
#include <functional>
class Task {
private:
    using func_t = std::function<int (int, int, char)>;
private:
    int x;
    int y;
    char op;
    func_t task;
public:
    Task() {   }
    Task(int x, int y, char op, func_t task) :
    x(x), y(y), op(op), task(task) {}

    std::string operator()() {
        int ret = task(x, y, op);
        char buf[1024];
        snprintf(buf, sizeof buf, "%d %c %d = %d", x, op, y, ret);
        return buf;
    }
    std::string taskToString() {
        char buf[1024];
        snprintf(buf, sizeof buf, "%d %c %d = ?", x, op, y);
        return buf;
    }
};

int myMath(int x, int y, char op) {
    int ret = -65535;
    switch (op)
    {
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
        if (0 == y) {
            std::cerr << "除0错误\n";
        }
        else {
            ret = x / y;
        }
        break;
    case '%':
        if (0 == y) {
            std::cerr << "余0错误\n";
        }
        else {
            ret = x % y;
        }
        break;
    default:
        std::cerr << "无法识别的操作符\n";
        break;
    }

    return ret;
}

std::string opers = "+-*/%";