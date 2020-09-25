#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

int main (int argc, char** argv) {

    char* file_name = argv[1];
    char* command_name = argv[2];


    assert(file_name);
    assert(command_name);

    char** new_argv = argv + 2;

    int fd = open(file_name, O_RDONLY);

    assert(fd != -1);  

    dup2(fd, STDIN_FILENO);

    execvp(command_name, new_argv);

    fprintf(stderr,"Could not execute execlp");

    return 0;
}