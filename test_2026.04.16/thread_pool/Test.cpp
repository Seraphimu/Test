#include "Thread.hpp"

void * thread(void * args) {
    printf("新线程启动成功.\n");

    return nullptr;
}

void test() {
    ThreadNs::Thread t;
    t.start(thread, (void *)123);
    t.join();
}

int main(void) {
    test();

    return 0;
}