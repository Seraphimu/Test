/**
 * 核心思想: 输入一条命令, 创建一个子进程, 完成程序替换, 并执行该命令
 * plus版本 clear
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX 1024
#define OPT_NUM 64
// 重定向类型枚举
#define NONE_REDIR   0
#define INPUT_REDIR  1
#define OUTPUT_REDIR 2
#define APPEND_REDIR  3

#define DEBUG

//如果isspace()返回非0值, 那么*start一定是空格
//本段代码执行到*start首个非空格内容为止
//比如ls >   txt, 一直走到: txt为止, 退出while循环
#define trimSpace(start) do {\
    while (isspace(*start)) start++;  \
}while(0)

//特性: 变量不能做数组名, 常变量是只读变量, 它是变量
// const int MAX = 1024;
// char lineCommand[MAX];

//用户输入的命令行
char lineCommand[MAX];
//存储选项指针
char * myArgv[OPT_NUM];
//重定向类型, 默认没有重定向
int redirType = NONE_REDIR;
//重定向文件名
char * redirFile = NULL;

int lastCode = 0;
int lastSig = 0;


void shell();
void commandCheck(char * command);

int main() {

    shell();

    return 0;
}

void shell() {
    while (1) {
        redirType = NONE_REDIR;
        redirFile = NULL;
        printf("%s@ %s %s#", getenv("USER"), "Xen", getenv("PWD"));
        fflush(stdout);

        char * s = fgets(lineCommand, sizeof(lineCommand) - 1, stdin);
        assert(s != NULL);
        (void)s;

        lineCommand[strlen(lineCommand) - 1] = 0;
        commandCheck(lineCommand);

        //以空格分离命令与选项
        // return a pointer to the next token, or NULL if there are no more tokens.
        myArgv[0] = strtok(lineCommand, " ");
        int i = 1;
        while (myArgv[i++] = strtok(NULL, " "));

//特殊处理
        //cd命令, 使用chdir代替
        if (myArgv[0] != NULL && strcmp(myArgv[0], "cd") == 0) {
            //chdir()  changes  the current working directory of the calling process 
            // to the directory specified in path.
            if (myArgv[1] != NULL) {
                chdir(myArgv[1]);
                //使用contineu跳到开头
                //能否解决PWD环境变量不的新 问题？
                /**
                 * 环境变量继承父进程，此模拟实现的cd命令，没有创建新的进程，修改了进程的当前路径
                 * 使用getenv()获取环境变量时，防尘吃鸡却没有任何改变。
                 */
                // printf("getenv(\"PWD\") = %s\n", getenv("PWD"));
                continue;               
            }
        }

        //无法打印环境变量
        //现在也无法打印，只是手动处理
        if (myArgv[0] != NULL && myArgv[1] != NULL && 0 == strcmp(myArgv[0], "echo")) {
            if (!strcmp(myArgv[1], "$?")) {
                printf("退出码: %d\n", lastCode);
                printf("退出信号: %d\n", lastSig);
            }
            else {
                ;
            }
            //阻止后续创建子进程
            continue;
        }

#ifdef DEBUG
        printf("命令: %s\n", myArgv[0]);
        i = 1;
        while (myArgv[i] != NULL) {
            printf("选项: %s\n", myArgv[i++]);
        }
        printf("重定向: %s\n", redirFile);
#endif

//创建子进程, 进行程序替换
        pid_t id = fork();
        assert(id >= 0);

        if (id == 0) {

            //处理文件重定向
            switch (redirType) {
                case NONE_REDIR:
                    break;
                case INPUT_REDIR:
                {
                    //打开文件, 并重定向到stdin
                    int fd = open(redirFile, O_RDONLY);
                    assert(fd >= 0);
                    //把stdin的内容输入到fd中去
                    //ls < 路径，和直接使用ls 路径区别不大
                    dup2(fd, 0);
                    break;
                }
                case OUTPUT_REDIR:
                case APPEND_REDIR:
                {
                    umask(0);
                    int flags = O_WRONLY | O_CREAT;
                    if (redirType == APPEND_REDIR)
                        flags |= O_APPEND;
                    else
                        flags |= O_TRUNC;
                    int fd = open(redirFile, flags, 0666);
                    assert(fd >= 0);
                    //让fd接管stdout, 把输出到屏幕上的字符全都输入到文件中
                    dup2(fd, 1);
                }
                default:
                    printf("bugs\n");
                    break;
            }
            execvp(myArgv[0], myArgv);
        }
        int status = 0;
        pid_t ret = waitpid(id, &status, 0);
        assert(ret > 0);

        //退出码和退出信号
        lastCode = ((status >> 8) & 0XFF);
        lastSig = (status & 0x7F);

//        printf("退出码: %d\n", lastCode);
//        printf("退出信号: %d\n", lastSig);
    }



}

void commandCheck(char * command) {
    assert(command);
    //拿到字符串的头和尾指针
    //假设一个字符串"abcde\0"
    //'a'的起始地址是0, 那么strlen(s) = 5, 
    //0 + 5 = 6, 即'\0'的位置
    //复习: C/C++中字符串以\0结尾, 但这仅仅只是语言层面上的规定
    char * start = command;
    char * end = command + strlen(command);

    while (start < end) {
        if (*start == '>') {
            //好比ls > txt
            //==> "ls \0" "txt" 
            *start = 0;
            start++;
            if (*start == '>') {
                //追加重定向
                redirType = APPEND_REDIR;
                start++;
            }
            else {
                redirType = OUTPUT_REDIR;
            }
            //跳过重定向文件前的空格
            trimSpace(start);
            //指向重定向文件的文件名
            redirFile = start;
            //处理完毕
            break;
        }
        else if (* start == '<') {
            //分离命令与重定向文件
            *start = '\0';
            start++;
            trimSpace(start);
            redirType = INPUT_REDIR;
            redirFile = start;
            break;
        }
        else {
            start++;
        }
    }
}