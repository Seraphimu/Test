#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>


void testSignal();
void testSignalHanlder(const int sig);

int main(int argc, char * argv[]) {
	
	testSignal();

	return 0;
}



void testSignal() {
	//sighandler_t signal(int signum, sighandler_t handler);
	//作用：绑定信号与处理信号的函数，与Qt框架的信号槽机制极大相似
	signal(2, &testSignalHanlder);
	while (1) {
		std::cout << "I am running" << std::endl;
		sleep(1);
	}
}

void testSignalHanlder(const int sig) {
	std::cout << "I have received signal: " << sig << std::endl;
}
