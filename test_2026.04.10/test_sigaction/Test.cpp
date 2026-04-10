#include <iostream>
#include <cstring>
#include <unistd.h>
#include <signal.h>

void commonHandler(int sig);
void commonHandlerEx(int sig);
//replace signal() with sigaction()
void replaceSignal();
// 捕捉期间屏蔽其他信号
void sigactionSamask();

void testSigaction();
void testSignal();

int main(void) {
	// replaceSignal();
	// sigactionSamask();
	// testSigaction();
	testSignal();

	return 0;
}
/**
 * 这里一个是结构体，一个是函数名，极易混乱
 */
void replaceSignal() {
	std::cout << "Pid: " << getpid() << ", 等待SIGINT" << std::endl;
	//创建struct sigaction类型对象
	struct sigaction act;
	//1. 初始化
	//1.1 使用memset()
	//memset(&act, 0, sizeof act);
	//1.2 使用sigemptyset()
	sigemptyset(&act.sa_mask);

	//2. 设置处理函数
	act.sa_handler = &commonHandler;
	//3. 设置行为标志
	act.sa_flags = 0;
	//4. 注册信号处理动作
	int ret = sigaction(SIGINT, &act, nullptr);
	if (ret == -1) {
		perror("sigaction");
		exit(-1);
	}

	while (1) {
		sleep(1);
		std::cout << "Running" << std::endl;
	}
}

void commonHandler(int sig) {
	std::cout << "I have received a signal: " << sig << std::endl;
	sigset_t pending;
	sigemptyset(&pending);
	int cnt = 10;
	while (cnt) {
		sigpending(&pending);
		std::cout << "SIG = " << sigismember(&pending, 2) << std::endl;
		std::cout << cnt << "s" << std::endl;
		cnt--;
		sleep(1);
	}
}

void sigactionSamask() {
	std::cout << "Pid: " << getpid() << std::endl;
	std::cout << 
	"1. 按下Ctrl + C" <<
	"2. 按下Ctrl + \\"
	<< std::endl;

	struct sigaction act;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGQUIT);
	sigaddset(&act.sa_mask, SIGINT);

	act.sa_handler = &commonHandlerEx;
	act.sa_flags = 0;

	//替代signal信号，绑定SIGINT信号与act.sa_handler
	sigaction(SIGINT, &act, nullptr);
	struct sigaction quitAct;
	sigemptyset(&quitAct.sa_mask);
	quitAct.sa_handler = SIG_DFL;
	quitAct.sa_flags = 0;
	sigaction(SIGQUIT, &quitAct, nullptr);
	while (1) {
		std::cout << "Running" << std::endl;
		sleep(1);
	}

}

void commonHandlerEx(int sig) {
	std::cout << "Starting Wait 5s" << std::endl;
	for (int i = 0; i < 5; i++) {
		sleep(1);
		std::cout << "Wait" << i << "s" << std::endl;
	}
	std::cout << "Complete" << std::endl;
}

/**
 * 先给进程发2号信号，再给进程发3号信号，再给进程发几次2号信号
 * 现象是：所有的2号信号被处理完之后，进程才会处理3号信号
 * 过程：
 * 当进程收到2号信号，调用自定义handler方法处理时，进程的block信号集2号会被置1
 * 即当前进程屏蔽2号信号，也就是说，在处理当前2号信号时，后续2号信号不会被递达。
 * 但该操作还是会在pending位图中把2号位修改成1，由于位图只有一份，后续发送2号信号给进程也只是做由1置1这个动作
 * 而进程对当前2号信号处理完毕之后，会立刻将pending中的2号信号拿去处理。
 * 又由于进程设置了：处理2号信号时，同时屏蔽3号信号，即后续的3号信号会在进程处理2号信号期间处于pending状态
 * 当2号信号被处理完毕，即pending2号位为空的时候，才会解除对3号信号的block。
 * 只要在处理2号信号期间一直发送2号信号，3号信号就不会得到处理。
 */
void testSigaction() {
	struct sigaction act, oact;
	memset(&act, 0, sizeof act);
	memset(&oact, 0, sizeof act);
	//设置自定义捕捉
	act.sa_handler = &commonHandler;
	sigemptyset(&act.sa_mask);
	//在捕捉指定信号SIGINT的同时屏蔽3号信号
	sigaddset(&act.sa_mask, 3);
	sigaction(SIGINT, &act, &oact);

	while (1) {
		std::cout << "Runing" << std::endl;
		sleep(1);
	}

}

//模拟失败
void testSignal() {
	sigset_t block, oblock;
	sigset_t pending;
	
	sigemptyset(&block);
	sigemptyset(&oblock);
	sigemptyset(&pending);

	sigaddset(&block, 3);
	sigprocmask(SIG_BLOCK, &block, &oblock);

	signal(2, &commonHandler);

	while (1) {
		// //模拟sigaction
		// sigpending(&pending);
		// //如果2号信号处理完了，就解除对3号信号的屏蔽
		// if (!sigismember(&pending, 2)) {
		// 	sigprocmask(SIG_SETMASK, &oblock, &block);
		// }
		std::cout << "Running" << std::endl;
		sleep(1);
	}

	// int cnt = 10;
	// while (1) {
	// 	if (0 == cnt--) {
	// 		std::cout << "不屏蔽任意信号" << std::endl;
	// 		sigprocmask(SIG_SETMASK, &oblock, &block);
	// 	}
	// }
}