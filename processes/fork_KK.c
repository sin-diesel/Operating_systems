#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv, char** envp) {

    pid_t pid = 0;
    pid_t ppid = 0;

    int a = 0;

    pid = fork();

    a += 1;

    if (pid == 0) { // child
        execle("/Users/stassidelnikov/Documents/MIPT/Operating_systems/processes/processes",
                "processes", (char*) NULL,
                envp);
        fprintf(stderr, "Fatal error occured\n");
        exit(0);
    }


    //pid = getpid();
    //ppid = getppid();

    //fprintf(stdout, "PID: %d, PPID: %d, resilt: %d\n", (int) pid, (int) ppid, (int) a);

    return 0;
}