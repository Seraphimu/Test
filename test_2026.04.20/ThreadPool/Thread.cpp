#include "Thread.hpp"
#include <iostream>
#include <vector>
#include <iterator>
void * start_routine(void * args) {
    std::cout << "I am " << (long)args;
    std::cout << ", I am start\n";
    return nullptr;
}
const int MAX = 6;
int main(void) {

    std::vector<Thread> threads;
    
    for (long i = 0; i < MAX; i++) {
        Thread t(&start_routine, (void *)i);
        threads.push_back(t);

    }
    std::vector<Thread>::iterator threadsIter = threads.begin();
    
    while (threadsIter != threads.end()) {
        threadsIter->start();
        ++threadsIter;
    }

    for (long i = 0; i < MAX; i++) {
        threads[i].join();
    }

    return 0;
}