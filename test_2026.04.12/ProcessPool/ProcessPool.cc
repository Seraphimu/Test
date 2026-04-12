#include "ProcessPool.h"

void downLoadTask()
{
    std::cout << getpid() << ": 下载任务\n"
              << std::endl;
    sleep(1);
}

void ioTask()
{
    std::cout << getpid() << ": IO任务\n"
              << std::endl;
    sleep(1);
}

void flushTask()
{
    std::cout << getpid() << ": 刷新任务\n"
              << std::endl;
    sleep(1);
}

void loadTaskFunc(std::vector<func_t> * out) {
    assert(out);
    out->push_back(&downLoadTask);
    out->push_back(&ioTask);
    out->push_back(&flushTask);
}

/**
 * 要在fork()之前就创建管道
 * 关文件描述符的时候一定要确保动作在父进程之内
 * 
 * 因为父进程创建管道时，总是读端和写端一起创建，并且接一个文件描述符，而在创建管道后总是会关闭读端，假设读端为3：
    第一次创建管道时：读3写4，然后父进程关闭读端，保留写端
    第二次创建管道时：读3写5，然后父进程关闭读端，保留写端
    第十次创建管道时：读3写13，然后父进程关闭读端保留写端
    又因为这些写端的管道的文件描述符全都被放在了deleteFd中，而后在子进程中关闭它们，但是，从父进程那里fork()过来的读端永远被保留！
    当然也不一定总是3，这要看父进程第一次的读写端分配的是几，反正不是3就是4.
    如果父进程抽风，每次调用pipe时都用较大的来当读端，那子进程读端fd的下标很可能会非常大。不过这份代码的运气很好，刚才创建了10000个进程，它还是3
 */

void createSubProcess(std::vector<SubEp> * subs, std::vector<func_t> & funcMap) {
    //批量创建子进程
    //要释放掉子进程多余的文件描述符, 父进程是跑不掉的, 
    //但子进程可以直接释放到[父进程文件描述符的数量, 3]
    std::vector<int> deleteFd;
    for (int i = 0; i < PROCESS_NUM; i++) {
        int fds[2];
        int ret = pipe(fds);
        assert(ret == 0);

        int id = fork();
        assert(id >= 0);

        if (id == 0) {
            //释放多余的文件描述符
            for (int i = 0; i < deleteFd.size(); i++) {
                std::cout << "子进程" << getpid() << "关闭了: " << deleteFd[i] << std::endl;
                std::cout << "子进程" << getpid() << "读端: " << fds[0] << std::endl;
                close(deleteFd[i]);
            }
            //关闭子进程写端
            close(fds[1]);
            while (true) {
                int commandCode = recvTask(fds[0]);
                //判断命令是否有效, 有效则执行
                if (commandCode >= 0 && commandCode < funcMap.size()) {
                    funcMap[commandCode]();
                }
                else if (commandCode == -1) {
                    break;
                }
            }
            //进程执行完就赶紧退出, 避免越建越多
            exit(0);
        }

        //关闭父进程读端
        close(fds[0]);
        //创建一个sub对象
        SubEp sub(id, fds[1]);
        subs->push_back(sub);
        //保存所有创建的写端文件描述符:3, 4, 5, 6...
        deleteFd.push_back(fds[1]);
    }
}

int recvTask(int readFd) {
    //从读端获取一个4字节的命令码
    int code = 0;
    ssize_t r = read(readFd, &code, sizeof code);
    if (r == 4) {
        //成功获取一个整数
        return code;
    }
    else if (r <= 0) {
        return -1;
    }
    else {
        return 0;
    }
}

void sendTask(const SubEp &process, int taskNum) {
    std::cout << "任务编号: " << taskNum <<
    ", 发送给" << process.name << std::endl;
    int w = write(process.writeFd, &taskNum, sizeof taskNum);
    assert (w == sizeof (int));
    (void)w;
}

void loadBalanceControl(const std::vector<SubEp> & subs, 
    const std::vector<func_t> & funcMap, int cnt)
{
    //统计数量
    int processNum = subs.size();
    int taskNum = funcMap.size();

    bool forever = (cnt == 0 ? true : false);
    
    while (true) {
        // 让任意一个进程, 执行任意一个任务
        int subIdx = rand() % processNum;
        int taskIdx = rand() % taskNum;
        sendTask(subs[subIdx], taskIdx);
        sleep(1);

        if (!forever) {
            cnt--;
            if (cnt == 0) {
                break;
            }
        }
    }
    //关闭所有写端管道
    for (int i = 0; i < processNum; i++) {
        close(subs[i].writeFd);
    }
}

void waitProcess(const std::vector<SubEp> & subs) {
    for (int i = 0; i < subs.size(); i++) {
        int id = waitpid(subs[i].subId, nullptr, 0);

        if (id > 0) {
            std::cout << "等待: " << id << "成功" << std::endl;
        }
    }
}