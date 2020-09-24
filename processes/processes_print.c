#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define WAIT

void get_info() {

    int pid = getpid();
    int ppid = getppid();


    fprintf(stdout, "PID: %d PPID: %d\n", pid, ppid);
}


int main () {

    int* status = NULL;

    int pid = 0;

    int result = 0;

    for (int i = 0; i < 3; ++i) {

        pid = fork();

        #ifdef WAIT
        wait(status);
        #endif


    }

    get_info();

    return 0;
}