#pragma once

#include <iostream>

class Task {
public:
    Task(const int a = 0, const int b = 0) {
        this->a = a;
        this->b = b;
    }
    void run() {
        std::cout << "T_" << pthread_self() << "执行" <<
        a << "+" << b << "\n";
    }
private:
    int a;
    int b;
};