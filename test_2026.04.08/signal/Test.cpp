#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void testHandlerCommon(const int sig);

void testSignal();
//自定义动作
void testSignalHanlder(const int sig);

void testCoreDump();

//-------------通过系统调用产生信号--------------------
void testKill_9(int argc, char * argv[]);

//-------------通过库函数产生信号--------------------
void testRaise(int argc, char * argv[]);
void testRaiseHandler(int sig);

void testAbort();

//-------------通过软件产生信号--------------------
void testAlarm();
void testAlarmPrint();
void testAlarmPrintHandler(int sig);
long long int cnt = 1;

//-------------通过硬件产生信号--------------------
void testDivZero();
void testWildPointer();

//信号集
void testPrintPending(sigset_t* pend);
void testPending();

int main(int argc, char * argv[]) {
	
	//testSignal();
	//testCoreDump();
	//testKill_9(argc, argv);
	//testRaise(argc, argv);
	//testAbort();
	//testAlarm();
	//testAlarmPrint();
	//testDivZero();
	testWildPointer();
	//testPending();

	return 0;
}


void testHandlerCommon(const int sig) {
	std::cout << "I have received signal: " << sig << std::endl;
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

void testCoreDump() {
	while (1) {
		std::cout << "I am running" << std::endl;
	}
}

void testKill_9(int argc, char * argv[]) {
	if (argc == 3) {
		kill(atoi(argv[1]), atoi(argv[2]));
	}
}


void testRaise(int argc, char * argv[]) {
	const int SIG_NUM = atoi(argv[1]);
	//进程给自己发信号
	//3号手册int raise(int sig);
	//即使绑定了9号信号的处理方式，进程照样会被kill掉
	signal(SIG_NUM, &testRaiseHandler);

	while (1) {
		std::cout << "I am running" << std::endl;
		sleep(1);
		raise(SIG_NUM);
	}
}

void testRaiseHandler(int sig) {
	std::cout << "I have receivced a signal: " << sig << std::endl;
}

void testAbort() {
	//#include <stdlib.h>
	//[[noreturn]] void abort(void);
	//或者使用signal(9, &test);
	signal(SIGABRT, &testHandlerCommon);	
	while (1) {
		abort();
	}
}

void testAlarm() {
	alarm(1);
	while (1) {
		std::cout << "I am running " <<  cnt << std::endl;
		cnt++;
	}
}

void testAlarmPrint() {
	signal(14, &testAlarmPrintHandler);
	alarm(1);

	while (1) {
		cnt++;
	}

}

void testAlarmPrintHandler(int sig) {
	std::cout << "cnt = " << cnt << std::endl;
	exit(0);
}

void testDivZero() {
	int a = 100;
	int b = 0;
	int c = a/b;
//	int id = fork();
//	
//	if (id == 0) {
//		int a = 100;
//		int b = 0;
//		a/b;
//	}
//	else if (id > 0) {
//		int status = 0;
//		size_t w = waitpid(id, &status, 0);
//		if (w > 0) {
//			std::cout << "The Child Process be killed by signal: " << (status & 0x7f) << std::endl;
//			std::cout << "pid = " << w << std::endl;
//			std::cout << "pid = " << id << std::endl;
//
//		}
//	}
//	else {
//		perror("fork");
//		exit(-1);
//	}
//
}
void testWildPointer() {
	//signal(11, &testHandlerCommon);
	int * p = nullptr;
	std::cout << *p << std::endl;
}

void testPrintPending(sigset_t* pend) {
  int i = 0;
  for (i = 1; i < 32; ++i)
  {
    if (sigismember(pend, i)){
      printf("1");
    }
    else{
      printf("0");
    }
  }
  printf("\n");
}
void testPending()
{
  sigset_t set, oset;
  sigset_t pending;
  // 使用系统函数对信号集进行初始化
  sigemptyset(&set);
  sigemptyset(&oset);
  sigemptyset(&pending);

  // 阻塞2号信号
  // 先用系统函数对set信号集进行设置
  sigaddset(&set, 2);
  // 使用sigprocmask函数更改进程的信号屏蔽字
  // 第一个参数，三个选项：SIG_BLOCK(mask |= set) SIG_UNBLOCK(mask &= ~set) SIG_SETMASK(mask = set)
  sigprocmask(SIG_BLOCK, &set, &oset);
  
  int flag = 1; // 表示已经阻塞2号信号
  int count = 0;
  while (1){
    // 使用sigpending函数获取pending信号集
    sigpending(&pending);
    // 打印pending位图
    testPrintPending(&pending);
    sleep(1);
  }
}
