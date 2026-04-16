#pragma once

#include <iostream>
#include <queue>
#include <algorithm>
#include <ctime>
#include <pthread.h>
#include <unistd.h>

class Task {
private:

public:
};


int myMath(const int x, const int y, const char op) {
    int result = 0;
    switch (op) {
        case '+':
            result = x + y;
            break;
        case '-':
            result = x - y;
            break;
        case '*':
            result = x * y;
            break;
        case '/': {
            if (y == 0) {
                std::cerr << "除0\n";
                result = -1;
            }
            else {
                result = x / y;
            }
            break;
        }
        case '%': {
            if (y == 0) {
                std::cerr << "模0\n";
                result = -1;
            }
            else {
                result = x % y;
            }
            break;
        }
        default:
            break;

    }

    return result;
}