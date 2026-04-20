#include <string>
#include <cassert>
#include <functional>
#include <pthread.h>
class Thread {
private:
    using func_t = std::function<void * (void *)>;
    const int MAX_BUF = 64;
private:
    pthread_t m_Tid;
    std::string m_Name;
    func_t m_CallBack;
    void * m_Args;
    static int m_Cnt;
public:
    Thread(func_t callBack, void *args = nullptr) {
        char buf[MAX_BUF];
        snprintf(buf, sizeof buf, "T%d", m_Cnt++);
        m_Name = buf;
        this->m_CallBack = callBack;
        this->m_Args = args;
    }

    void start() {
        int ret = pthread_create(&m_Tid, nullptr, &start_routine, (void *)this);
        assert(ret == 0);
    }
    void join() {
        pthread_join(m_Tid, nullptr);
    }
private:
    static void * start_routine(void * args) {
        Thread * threadPtr = (Thread *)args;
        void * ret = threadPtr->m_CallBack(threadPtr->m_Args);
        return ret;
    }
};

int Thread::m_Cnt = 0;