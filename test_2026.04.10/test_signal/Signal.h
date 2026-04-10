#include <iostream>
#include <cstring>
#include <signal.h>
// #include <unistd.h>

void testCtrlC();
void testCtrlCHandler(int sig);

void testKill(int argc, char * argv[]);