#include "Signal.h"

void testCtrlC() {
    signal(2, &testCtrlCHandler);
    while (1) {
        std::cout << "Running" << std::endl;
        sleep(1);
    }
}

void testCtrlCHandler(int sig) {
    std::cout << "I have received a signal: " << sig << std::endl;
}


void testKill(int argc, char * argv[]) {
    //Received a signal from main() function
    if (argc == 3) {
        kill(atoi(argv[1]), atoi(argv[2]));
    }

    else {
        printf("用法: ./a.out [pid] [信号编号]");
    }
}