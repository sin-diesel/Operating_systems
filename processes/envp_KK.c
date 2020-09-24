#include <unistd.h>
#include <stdio.h>

int main (int argc, char** argv, char** envp) {

    fprintf(stdout, "Argc: %d\n", argc);

    for (int i = 0; i < argc; ++i) {

        fprintf(stdout, "argv[%d] = %s\n", i, argv[i]);
    }

    int i = 0;

    for (char** env = envp; *env != NULL; ++env) {
        ++i;
        fprintf(stdout, "envp[%d] = %s\n", i, *env);
    }
    return 0;
}