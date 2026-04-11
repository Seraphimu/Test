#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char * argv[]) {

    if (argc == 2) {
        //atoi()把字符串转成整数
        int closeFd = atoi(argv[1]);
        if (closeFd == 0 || closeFd == 1 || closeFd == 2) {
            //关闭0, 1, 2任意一个fd
            close(closeFd);

            //此时打开文件，并打印fd
            int fd = open("log.txt", O_WRONLY);
            assert(fd >= 0);
            
            printf("新打开文件的fd: %d\n", fd);

        }
    }
    else {
        printf("./a.out [未使用open打开任意文件前要关闭的文件fd(0, 1, 2)]\n");
    }

    return 0;
}

// int main(void) {
//     umask(0);
//     int fd = open("log.txt", O_WRONLY | O_CREAT, 0666);
//     assert(fd >= 0);

//     printf("fd = %d\n", fd);

//     return 0;
// }