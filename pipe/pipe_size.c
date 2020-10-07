#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

int main() {


    char const hello_str[] = "Hello, world!";
    unsigned const hello_size = 14;
    unsigned const buf_size = 600000;

    int fd[2] = {0, 0};
    size_t size = 0;

    int result = pipe(fd);
    assert(result == 0);

    int write_size = write(fd[1], hello_str, hello_size);
    assert(write_size == hello_size);

    char* ret_str = (char*) calloc(hello_size, sizeof(char));
    size_t read_size = read(fd[0], ret_str, hello_size);

    //fprintf(stderr, "read_str = %s\n", ret_str);
    assert(read_size == hello_size);

    free(ret_str);

    // Now testing pipe size

    char* buf = (char*) calloc(buf_size, sizeof(char));
    assert(buf);
    fprintf(stderr, "Buf created\n");

    int status = 0;
    pid_t pid = fork();
    
    if (pid == 0) { // try to write in pipe while in child
        close(fd[0]);
        size_t written_size = write(fd[1], buf, buf_size);
        assert(written_size == buf_size);
    } else {
        close(fd[1]);
        size_t read_size = read(fd[0], buf, 1);
        fprintf(stderr, "read: %d\n", read_size);
    }


    return 0;




}