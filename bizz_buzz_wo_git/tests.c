#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main () {
    FILE* test_file = fopen("tests.txt", "w");
    assert(test_file);

    unsigned n_tests = 1000;
    unsigned range = 1000;
    srand(time(NULL));
    for (int i = 0; i < n_tests; ++i) {
        int num_1 = rand() % range;
        int num_2 = rand() % range;
        int num_3 = rand() % range;
        int num_4 = rand() % range;
        fprintf(test_file, "%d %d %d %d\n", num_1, num_2, num_3, num_4);
    }
    return 0;
}