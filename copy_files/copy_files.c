#include <stdio.h>
#include <assert.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>




void copy(char const* from, char const* to) {


    int input = open(from, O_RDONLY);
    int output = open(to, O_WRONLY | O_CREAT, S_IRWXU);

    if (input < 0) {
        fprintf(stderr, "An error has occured when opening file being copied\n"); // fix, handle errno and not simply input
        exit(0);
    }

    if (output < 0) {
        fprintf(stdout, "An error has occured when opening the file\n"); // same as above
        if (errno == EISDIR) {
            fprintf(stderr, "An attempt to create file %s with an existing directory name\n", to);
        }
        exit(0);
    }

    struct stat from_info;
    fstat(input, &from_info);

    int from_size = from_info.st_size;

    fprintf(stderr, "from_size: %d\n", from_size); // fix

    void* buf = calloc(from_size, sizeof(char));

    assert(buf);

    int read_bytes = read(input, buf, from_size);

    assert(read_bytes != -1);

    fprintf(stderr, "read_bytes: %d\n", read_bytes); // fix

    int ret_bytes = write(output, buf, from_size); // overwriting

    assert(ret_bytes == read_bytes);

    struct stat to_info;
    fstat(output, &to_info);

    fprintf(stdout, "ret_bytes write: %d\n", ret_bytes);
    fprintf(stdout, "fstat output info: %lld\n", to_info.st_size);

    if (ret_bytes != to_info.st_size) {
        fprintf(stdout, "Error has occured during file copying"); // fix
    }

    free(buf);
    close(input);
    close(output);
}

int main(int args, char** argv) {

    if (argv[1] == NULL || argv[2] == NULL) { // fix
        fprintf(stderr, "Not enough arguments\n");
        exit(0); // fix
    }

    char* from = argv[1]; // file_22
    char* to = argv[2]; // file_4

    copy(from, to);




    return 0;
}