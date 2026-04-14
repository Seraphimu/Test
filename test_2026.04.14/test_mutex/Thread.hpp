#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <cstring>
#include <cassert>
#include <pthread.h>
#include <unistd.h>

typedef std::function<void *(void *)> func_t;
// typedef void *(*func_t)(void *);

/**
 * 这个接口设计得非常不优雅.
 */
class Thread {
private:
	std::string name;
	pthread_t tid;
	func_t task;
	void * args;
	
public:
	Thread(func_t task, void * args = nullptr, int num = 0)
	{
		this->task = task;
		this->args = args;
		char buf[64];
		snprintf(buf, sizeof buf, "T%d", num);
		name = buf;

		//这里不能直接使用task
		int ret = pthread_create(&tid, nullptr, &thread_entry, (void *)this);
		assert(ret == 0);
		(void)ret;
	}

	std::string getName() {
		return name;
	}

	void show() {
		std::cout << "线程: " << this->name << "实例化完毕\n";
	}

	void join() {
		int n = pthread_join(tid, nullptr);
		assert(n == 0);
		(void)n;
	}

	// 由于普通函数有this缺省参数, 而static类型函数没有this指针参数
	// 所以需要创建一个新的对象! 但这一步static_cast<Thread *>有必要吗?
	// 这样转换之后参数还是原来的参数吗? 不应该
	// Thread * self = new Thread();
	// void * result = self->task(args)吗?
	//明白了, 把this指针作为对象传过来, 并获取其args以及函数task的地址, 然后构造一个纯种的函数指针
	//最重要的是, 它来自static函数, 但为什么在static函数中操作它不会报错呢, 其实归根到底, 
	//ptheadcreate的第三个参数不还是result吗?
	static void * thread_entry(void *args) {
		Thread * self = static_cast<Thread *>(args);
		//误会大了, 这一步才是真正去执行我派给线程的任务!
		//这个静态方法就是一个入口, 让pthread_create()创建并执行它, 然后在这个方法里
		//调用self->task()这个真正的函数, 也就是说, 不能直接调它, 那我就找一个能调的函数
		//然后在这个能调的函数中再调它!
		//对接口有误解!! 但下次不会了, 谁知道居然会被这个细节困到死
		void * result = self->task(self->args);
		return result;
	}
};

