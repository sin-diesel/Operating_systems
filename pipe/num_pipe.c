#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>


int main() {

    int fd[2];

    int result = pipe(fd);

    assert(result == 0);
    int status = 0;
    int num = 0;

    int pid = fork();

    if (pid != 0) {
        close(fd[0]);
        int res = fscanf(stdin, "%d", &num);
        assert(res == 1);
        write(fd[1], &num, sizeof(int));
        //wait(&status);
        //close(fd[1]);

    } else {
        close(fd[1]);
        size_t read_size = read(fd[0], &num, sizeof(int)); // if no data is present, the child waits
        assert(read_size > 0);
        fprintf(stdout, "num: %d\n", num * 10);
        //close(fd[0]);

    }
    return 0;
}