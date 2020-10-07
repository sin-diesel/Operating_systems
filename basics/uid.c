#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main () {
    fprintf(stdout, "UID: %d\n", getuid());
    fprintf(stdout, "GID: %d", getgid());
    return 0;
}

