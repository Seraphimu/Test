#pragma once
#include "Thread.hpp"
#include "LockGuard.hpp"
#include <vector>
#include <queue>
#include <mutex>
#include <iostream>
const int gNum = 5;

template <class T>
class ThreadPool;


//为什么他要封装这个类, 这样有什么好处? 
template <class T>
class ThreadData {
public:
	ThreadPool<T> * threadPool;		//指向线程池对象
	std::string name;				//线程名称
public:
	ThreadData(ThreadPool<T> * tp, const std::string & n) {
		this->threadPool = tp;
		this->name = n;
	}
};



template <class T>
class ThreadPool {
private:
	int num;						//线程池内的线程数
	std::vector<Thread *> threads;	//指向Thread的线程指针数组
	std::queue<T> taskQueue;		//共享资源, 任务队列, 需要加锁
	pthread_mutex_t mutex;			//锁
	pthread_cond_t cond;			//条件变量, 阻塞, 唤醒线程
	static ThreadPool<T> *tp; 		//指向全局唯一的线程池实例
	static std::mutex singleLock;	//C++的锁, 用来保护单例
	struct ThreadInfo {				//传参用, 把线程名传递过去
		ThreadPool * pool;
		std::string name;
	};

private:
	//创建num个线程对象, 并把它们的指针放入到threads中
	ThreadPool(const int & num = gNum) {
		this->num = num;
		pthread_mutex_init(&mutex, nullptr);
		pthread_cond_init(&cond, nullptr);
		for (int i = 0; i < this->num; i++) {
			threads.push_back(new Thread());
		}
	}
	static void * handlerTask(void * args) {
		//用局部变量拿到参数之后立刻释放空间
		ThreadInfo * info = static_cast<ThreadInfo *>(args);
		ThreadPool * tp = info->pool;
		std::string name = info->name;
		delete info;
		while (true) {
			T t;
			{
				LockGuard lockGuard(&tp->mutex);
				while (tp->isQueueEmpty()) {
					tp->threadWait();
				}
				t = tp->pop();
			}
			std::cout << name << " 获取任务: " << t.taskToString()
			<< ", 结果: " << t() << "\n";
		}
	}
	// static void * (void * args) {
	// 	ThreadPool * tp = static_cast<ThreadPool *>(args);
	// 	while (true) {
	// 		T t;
	// 		{
	// 			LockGuard lockGuard(&tp->mutex);
	// 			while (tp->isQueueEmpty()) {
	// 				tp->threadWait();
	// 			}
	// 			t = tp->pop();
	// 		}
	// 		std::cout << "获取任务: " << t.taskToString() << 
	// 		", 结果是: " << t() << "\n";
	// 	}
	// }
	// static void * handlerTask(void * args) {
	// 	ThreadData<T> * td = static_cast<ThreadData<T> *>(args);
	// 	while (true) {
	// 		T t;
	// 		{
	// 			LockGuard lockGuard(td->threadPool->getMutex());
	// 			while (td->threadPool->isQueueEmpty()) {
	// 				td->threadPool->threadWait();
	// 			}
	// 			//取出任务, 做了封装, 和front()再pop()效果一样
	// 			t = td->threadPool->pop();
	// 		}
	// 		std::cout << td->name << "获取任务: " << t.taskToString() << 
	// 		", 结果是: " << t() << "\n";
	// 	}
	// 	delete tp;
	// 	return nullptr;
	// }
public:
	~ThreadPool() {
		pthread_mutex_destroy(&mutex);
		pthread_cond_destroy(&cond);
		for (const auto & t : threads) {
			delete t;
		}
	}
	/**
	 * 单例设计模式, 创造出全局唯一的一个ThreadPool实例化对象
	 * 使用静态变量tp指向这个唯一的对象
	 * 如果tp为空, 创建, 反之直接返回tp
	 * 这样一来, 每次需要调用该类的方法, 直接getInstance()->func()即可
	 * 不需要再创建新的对象
	 */
	static ThreadPool<T> * getInstance() {
		if (nullptr == tp) {
			//cannot call member function without object
			// LockGuard lockGuard(*getMutex());
			singleLock.lock();
			if (nullptr == tp) {
				tp = new ThreadPool<T>();
			}
			singleLock.unlock();
		}
		return tp;
	}

	void run() {
		for (const auto & t : threads) {
			//为了传递更多信息? 
			// ThreadData<T> * td = new ThreadData(this, t->getName());
			ThreadInfo * ti = new ThreadInfo();
			ti->name = t->getName();
			ti->pool = this;
			t->start(&handlerTask, (void *)ti);
			std::cout << t->getName() << " has been started\n";

		}
	}

public:
	void lockQueue() { pthread_mutex_lock(&this->mutex); }
	void unlockQueue() { pthread_mutex_unlock(&this->mutex); }
	bool isQueueEmpty() { return taskQueue.empty(); }
	void threadWait() { pthread_cond_wait(&this->cond, &this->mutex); }
	T pop() {
		T t = taskQueue.front();
		taskQueue.pop();
		return t;
	}
	// 这玩意怎么用
	pthread_mutex_t * getMutex() {
		return &this->mutex;
	}
	void push(const T & in) {
		LockGuard lockGuard(&this->mutex);
		taskQueue.push(in);
		pthread_cond_signal(&cond);
	}
};

template<class T>
std::mutex ThreadPool<T>::singleLock;

template <class T>
ThreadPool<T> * ThreadPool<T>::tp = nullptr;