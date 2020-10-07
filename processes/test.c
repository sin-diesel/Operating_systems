#include <stdio.h>
#include <unistd.h>

int main () {
    for (int i = 0; i < 5; ++i) {
        fork();
        sleep(5);
        printf("Hi\n");
    }
    return 0;
}