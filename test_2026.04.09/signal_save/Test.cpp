#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <vector>

#define MAX_SIGNUM 31

std::vector<int> sigArr;

void testHandlerCommon(const int sig);
//信号集
void testPrintPending(sigset_t* pend);
void testPending();

void testSigAction();

//屏蔽信号
//代码逻辑：
//1. 设置阻塞信号集，设置完毕之后这些信号到达时不会被处理，会被保存到pending信号集中
//2. 给进程发对应的信号，并查看当前进程的pending信号集
void testBlock();
void show_pending(const sigset_t &pending);

void sigInit();
void testBlockEx();
void showPendingEx(const sigset_t & pending);
//自定义处理方案：在屏蔽上跑过一趟SL机车
//如果执行失败可以 sudo apt install sl 或者通过其他包管理器来安装
//之前一直以为只是简单的把ls写反了，还怪有意思的。
void handlerSL(int sig);

int main(int argc, char * argv[]) {
	
	// testPending();
	// testBlock();
	sigInit();
	testBlockEx();

	return 0;
}


void testHandlerCommon(const int sig) {
	std::cout << "I have received signal: " << sig << std::endl;
}

void testPending()
{
	sigset_t set, oset;
	sigset_t pending;
	// 使用系统函数对信号集进行初始化
	//置空
	sigemptyset(&set);
	sigemptyset(&oset);
	sigemptyset(&pending);

	// 阻塞2号信号
	// 先用系统函数对set信号集进行设置
	//add respectively signal signum from set.
	sigaddset(&set, 2);
	// 使用sigprocmask函数更改进程的信号屏蔽字
	// 第一个参数，三个选项：
	//SIG_BLOCK(mask |= set) SIG_UNBLOCK(mask &= ~set) SIG_SETMASK(mask = set)
	//sigprocmask()  is  used to fetch and/or change the signal mask of the calling thread. 
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


//The  sigaction() system call is used to change the action 
//taken by a process on receipt of a specific signal. 
//与signal()系统调用相似

void testSigAction() {
	struct sigaction act, oact;
	act.sa_flags = 0;
	sigfillset(&act.sa_mask);
}


//阻塞信号集也叫做当前进程的信号屏蔽字，这里的“屏蔽”应该理解为阻塞而不是忽略。
//测试1到31号信号哪个
void testPrintPending(sigset_t* pend) {
	int i = 0;
	for (i = 1; i < 32; ++i)
	{
		//sigismember() tests whether signum is a member of set.
		if (sigismember(pend, i)){
		printf("1");
		}
		else{
		printf("0");
		}
	}
	printf("\n");
}



void testBlock() {
	sigset_t block, oblock, pending;
	sigemptyset(&block);
	sigemptyset(&oblock);

	//向屏蔽信号集中添加屏蔽信号
	sigaddset(&block, 2);

	sigprocmask(SIG_SETMASK, &block, &oblock);

	//int cnt = 10;
	while (true) {
		sigemptyset(&pending);
		//sigpending通过输出型参数获取当前进程的pending信号集
		sigpending(&pending);
		show_pending(pending);
		sleep(1);
	}

}

void show_pending(const sigset_t &pending){
	for (int i = 1; i < 32; i++) {
		if (sigismember(&pending, i)) {
			std::cout << "1";
		}
		else {
			std::cout << "0";
		}
	}
	std::cout << std::endl;
}

void testBlockEx() {
	//给进程设置屏蔽信号集
	sigset_t block, oblock;
	//获取进程的pending信号集
	sigset_t pending;

	sigemptyset(&block);
	sigemptyset(&oblock);
	sigaddset(&block, 2);
//使用条件编译去搞不同的情况
	// //把1~31号信号全都屏蔽，并设置成自定义的“跑火车”方案。
	// //其中进程无法以跑火车为借口逃离被9号信号终止
	// //甚至连火车都跑不起来。
	// for (const auto & sig: sigArr) {
	// 	sigaddset(&block, sig);
	// 	// signal(sig, &handlerSL);
	// }

	sigprocmask(SIG_SETMASK, &block, &oblock);

	signal(2, &handlerSL);
	int cnt = 10;
	while (true) {
		sigemptyset(&pending);
		sigpending(&pending);
		showPendingEx(pending);

		sleep(1);
		if (0 == cnt--) {
			std::cout << "吉时已到，不屏蔽任意信号！" << std::endl;
			sigprocmask(SIG_SETMASK, &oblock, &block);
		}
	}
}

void showPendingEx(const sigset_t & pending) {
	for (int sig = MAX_SIGNUM; sig > 0; sig--) {
		if (sigismember(&pending, sig)) {
			std::cout << "1";
		}
		else {
			std::cout << "0";
		}
	}
	std::cout << std::endl;
}

void handlerSL(int sig) {
	testHandlerCommon(sig);
	int id = fork();

	//这里如果不用子进程来替换，整个进程就结束了
	if (id == 0) {
		execlp("sl", "sl", nullptr);
	}
}

void sigInit() {
	for (int i = 1; i <= MAX_SIGNUM; i++) {
		sigArr.push_back(i);
	}
}

  // //不知道它是具体做什么的，但我知道它是signal()++
  // sigaction(2, &act, &oact);
