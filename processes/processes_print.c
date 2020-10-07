#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define WAIT

void get_info(int pid, int num) {

    int status = 0;

    if (num < 0) {
        return;
    }

    if (pid == 0) { // child
        int new_pid = fork();
        get_info(new_pid, num - 1);
    } else {
        wait(&status);
        fprintf(stdout, "PID: %d PPID: %d num: %d\n", pid, getppid(), num);
        return;
    }


}


int main () {

    get_info(fork(), 8);

    return 0;
}