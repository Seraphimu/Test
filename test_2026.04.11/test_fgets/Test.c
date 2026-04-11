#define FGETS_FILE
// #define FILE_SYS_CALL

#ifdef FILE_SYS_CALL

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 1024
int main(void) {
    int fd = open("log.txt", O_RDONLY);
    assert(fd >= 0);

    //sizeof (type), 括号不能省略变量可以
    char * buf = (char *)malloc(sizeof (char) * MAX);
    ssize_t r = read(fd, buf, MAX - 1);
    write(1, buf, strlen(buf));

    close(fd);
    free(buf);

    return 0;
}

#endif





#ifdef FGETS_FILE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define MAX 64

//把文件中的数据读到屏幕上
int main(void) {

    FILE * fp = fopen("log.txt", "r");

    char buf[MAX] = "0";
    while (NULL != fgets(buf, sizeof buf, fp)) {
        printf("%s", buf);
    }

    // char buf[MAX] = "0";
    // while (NULL != fgets(buf + strlen(buf), sizeof buf, fp)) {
    //     ;
    // }
    // printf("%s", buf);


    //文件指针fp会控制读写位会不断后移，逐行地把文件读完：
    /**
     * 早期的C语言就采用这种"流(stream)"的概念：
     * - 文件就像水流，只能顺序读取
     * - 指针记录当前位置
     * - 每次读取都推进指针
     */
    // fgets(buf, sizeof buf, fp);
    // printf("%s", buf);
    // fgets(buf, sizeof buf, fp);
    // printf("%s", buf);

    fclose(fp);
    return 0;
}

#endif






#ifdef FGETS

#include <stdio.h>
#include <string.h>
#define MAX 64
int main(void) {
	char buf[MAX];
	char * str = fgets(buf, sizeof str, stdin);
	printf("1: %s", str);
	printf("2: %s", buf);

	// for (int i = 0; i < MAX; i++) {
	// 	printf("%c ", str[i]);
	// }
	// printf("\n");
	
	return 0;
}

#endif