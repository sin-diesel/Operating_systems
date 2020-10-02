#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

void usr1_handler(int signal) {
    printf("signal received!\n");
}

int main () {


    signal(SIGUSR1, &usr1_handler);

    while(1) {
        sleep(10);
    }


    return 0;
}