#include "Task.hpp"
#include "ThreadPool.hpp"
#include <ctime>
#include <unistd.h>


int main(void) {
    srand((size_t)time(nullptr));
    ThreadPool * tp = new ThreadPool;
    tp->threadPoolInit();

    while (true) {
        int x = rand() % 10 + 1;
        int y = rand() % 10 + 1;
        sleep(1);
        tp->putTask(new Task(x, y));
    }

    return 0;
}








// void testTask() {
//     Task t(114, 514);
//     t.run();
// }

// int main(void) {
//     testTask();

//     return 0;
// }