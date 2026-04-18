#include <iostream>
#include <string>

class ThreadPool {
public:
    struct ThreadInfo {
        int a;
        char b;
    };
    std::string name;
};

int main(void) {
    ThreadPool t;
    //public下可以直接这么干
    ThreadPool::ThreadInfo info;
    info.a = 11451;
    info.b = 'c';

    std::cout << "info.a = " << info.a << "\n";
    std::cout << "info.b = " << info.b << "\n";

    return 0;
}