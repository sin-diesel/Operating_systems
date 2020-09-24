#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main () {

    pid_t pid = 0;
    pid_t ppid = 0;

    int a = 0;

    fork();

    a += 1;

    pid = getpid();
    ppid = getppid();

    fprintf(stdout, "PID: %d, PPID: %d, resilt: %d\n", (int) pid, (int) ppid, (int) a);

    return 0;
}