#include "ThreadPool.hpp"
#include "Task.hpp"
#include <memory>
#include <ctime>
#include <unistd.h>

int main()
{
    srand((size_t)time(nullptr));
    //创建一个线程池, 全局唯一的实例, 并调用其run()函数
    ThreadPool<Task>::getInstance()->run();

    int x, y;
    char op;
    while(1)
    {
        x = rand() % 10 + 1;
        y = rand() % 10 + 1;
        op = oper[rand() % oper.length()];
        Task t(x, y, op, &myMath);
        ThreadPool<Task>::getInstance()->push(t);
        sleep(1);
    }
}














// void * run(void * args) {
// 	while (true) {
// 		std::cout << "hello\n";
// 		sleep(1);
// 	}
// }

// void testThread() {
// 	Thread t;
// 	t.start(&run, nullptr);
// 	t.join();
// }

// int main(void) {
// 	// testThread();
	
// 	return 0;
// }
