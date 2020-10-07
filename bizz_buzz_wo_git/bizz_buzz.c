#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

//#define NDEBUG

#define TEST 0

#define DBG(a) if (TEST) {a;}
#ifdef TEST
#define ASSERT_OK(error) fprintf(stderr, "ERROR AT LINE %d, FUNCTION %s\nCALLING DUMP\n", __LINE__, __func__); \
                         dump(error);                                                                          
#endif                                                                        

enum errors {
    NO_INPUT = -1,
    ARGS_OVERFLOW = -2,
    NOFILE = -100,
    READ_SIZE = -4,
    ARGS_UNDERFLOW = -5,
    BAD_ALLOC = -6,
};


int check_input(int argc, char** argv) {
    if (argc == 1) {
        return NO_INPUT;
    } else if (argc == 2) {
        return ARGS_UNDERFLOW;
    } else if (argc > 3) {
        return ARGS_OVERFLOW;
    }

    return 0;
}

void dump(int error_type) {
    switch(error_type) {

        case NO_INPUT:
        fprintf(stderr, "bizz_buzz error: at least 1 argument expected\n");
        exit(NO_INPUT);
        break;

        case ARGS_OVERFLOW:
        fprintf(stderr, "bizz_buzz error: too many arguments passed\n");
        exit(ARGS_OVERFLOW);
        break;

        case EISDIR:
        fprintf(stderr, "bizz_buzz error: trying to open a directory\n");
        exit(EISDIR);
        break;

        case ENOENT:
        fprintf(stderr, "bizz_buzz error: input file does not exist\n");
        exit(ENOENT);
        break;

        case ARGS_UNDERFLOW:
        fprintf(stderr, "bizz_buzz error: output file does not exist\n");
        exit(ARGS_UNDERFLOW);
        break;

        case BAD_ALLOC:
        fprintf(stderr, "bizz_buzz error: could not allocate memory\n");
        exit(BAD_ALLOC);
        break;


        case 0:
        break;

        default:
        fprintf(stderr, "bizz_buzz error: unexpected error\n");
    }
}


char* replace_str(char* buf, char* pos, const char* str, unsigned* buf_size, unsigned insert_sz, unsigned offset) {

    assert(buf);
    assert(pos);
    assert(str);

    unsigned pos_offset = pos - buf; // remembering position offset
 
    DBG(fprintf(stderr, "Buffer before replace:%s\n", buf))
    DBG(fprintf(stderr, "Buffer size before replace:%d\n", *buf_size))
    DBG(fprintf(stderr, "Inserted string size:%d\n", insert_sz))

    char* remaining = pos + offset; // part of buffer that needs to be appended after insertion
    int rem_len = strlen(remaining);
    char* copy_remaining = (char*) calloc(rem_len, sizeof(char));
    if (copy_remaining == NULL) {
        ASSERT_OK(BAD_ALLOC)
    }
    assert(copy_remaining);
    strcpy(copy_remaining, remaining);
    *buf_size = *buf_size + insert_sz + offset + 1; // increasing buffer size, may become bigger than required but not a huge issue

    DBG(fprintf(stderr, "Remaining buffer:%s\n", remaining))
    DBG(fprintf(stderr, "Remaining buffer (copied):%s\n", copy_remaining))
    DBG(fprintf(stderr, "New expected buffer size:%d\n", *buf_size))
    int rem_size = strlen(copy_remaining);
    DBG(fprintf(stderr, "New required space:%d\n", rem_size + insert_sz + offset))

    char* new_buf = realloc(buf, *buf_size);

    if (new_buf != buf) {
        DBG(fprintf(stderr, "BUFFER %s REALLOCATED to %s\n", buf, new_buf));
    }

    if (new_buf == NULL) {
        ASSERT_OK(BAD_ALLOC);
    }
    DBG(fprintf(stderr, "Buffer after realloc:%s\n", new_buf))
    strcpy(new_buf + pos_offset, str);
    DBG(fprintf(stderr, "Buffer after copying str:%s\n", new_buf))
    strcat(new_buf, copy_remaining);
    DBG(fprintf(stderr, "Buffer after adding remaining str:%s\n", new_buf))
    free(copy_remaining);
    return new_buf;
}

unsigned count_digits(long number) {
    unsigned count = 0;
    while (number != 0) {
        number /= 10;
        ++count;
    }
    return count;
}

void test_replace(char* buf, const char* str, int* buf_size, unsigned insert_sz) {
    replace_str(buf, buf + 3, str, (unsigned*) buf_size, insert_sz, 0);
    DBG(fprintf(stderr, "<<<< FINISHED STRING TEST >>>> \n\n\n"))
}

char* parse_numbers(char* buf, unsigned* size) {

    const char bizz[] = "bizz";
    const char buzz[] = "buzz";
    const char bizz_buzz[] = "bizzbuzz";

    char* p = buf;
    char** endptr = (char**) calloc(1, sizeof(char*)); // careful
    if (endptr == NULL) {
        ASSERT_OK(BAD_ALLOC)
    }
    assert(endptr);

    long number = 0;
    unsigned buf_offset = 0;

    while(*p) { // while there are charachters in buffer
        number = strtol(p, endptr, 10);
        unsigned digits = 0;

        DBG(fprintf(stderr, "Parsed number from buffer: %ld\n", number))
        DBG(fprintf(stderr, "Remaining buffer:%s\n", p))
        DBG(fprintf(stderr, "Invalid string begins with:%s\n", *endptr))

        if (number > 0) {

            digits = count_digits(number);
            DBG(fprintf(stderr, "Number %ld has %d digits\n", number, digits))

            if (number % 15 == 0) {

                p = *endptr - digits; // moving to the location preceeding string
                DBG(fprintf(stderr, "Replacing from:%s\n", p))
                buf = replace_str(buf, p, bizz_buzz, size, sizeof("bizzbuzz"), digits);

            } else if (number % 3 == 0) {

                p = *endptr - digits;
                DBG(fprintf(stderr, "Replacing from:%s\n", p))
                buf = replace_str(buf, p, bizz, size, sizeof("bizz"), digits);

            } else if (number % 5 == 0) {

                p = *endptr - digits;
                DBG(fprintf(stderr, "Replacing from:%s\n", p))
                buf = replace_str(buf, p, buzz, size, sizeof("buzz"), digits);
                
            }
    p = buf + buf_offset; // refreshing p pointer
        }

        strtol(p, endptr, 10); // refreshing endptr pointer
        if (number > 0) { // if a number was parsed
            p = *endptr;
            buf_offset = p - buf; // refrehing buf_offset
        } else {
            p++; // else simply moving p as no refreshing is required
            buf_offset++; 
        }
    }
    return buf;
}

void print_buf(char* buf, unsigned n_bytes) {
    int res = write(STDOUT_FILENO, buf, n_bytes);
    assert(res == n_bytes);
}

int handle_input(int argc, char** argv) {

    int fd_1 = open(argv[1], O_RDONLY);
    int fd_2 = NOFILE;

    if (fd_1 < 0) {
        ASSERT_OK(errno)
    }

    fd_2 = open(argv[2], O_WRONLY);

    if (errno == ENOENT) {
        fprintf(stdout, "bizz_buzz warning: output file not specified, creating new output file\n");
        fd_2 = open(argv[2], O_WRONLY | O_CREAT, S_IRWXU);
        assert(fd_2 > 0);
    }

    if (fd_2 < 0) {
        ASSERT_OK(errno)
    }

    struct stat input;
    fstat(fd_1, &input);

    int input_size = input.st_size;

    char* buf = (char*) calloc(input_size, sizeof(char));
    if (buf == NULL) {
        ASSERT_OK(BAD_ALLOC)
    }
    assert(buf);

    int n_read = read(fd_1, buf, input_size);

    buf[input_size] = '\0'; // null-terminating buffer

    if (n_read == -1) {
        ASSERT_OK(errno)
    }

    assert(n_read == input_size);

    if (n_read != input_size) {
        ASSERT_OK(READ_SIZE)
    }

    unsigned buf_size = (unsigned) n_read;

    DBG(fprintf(stderr, "Buf %p size before replace: %d\n", buf, buf_size)) 
    char* new_buf = parse_numbers(buf, &buf_size);
    int new_size = strlen(new_buf);
    DBG(fprintf(stderr, "Buf %p size after replace: %d\n", new_buf, new_size))

    int n_write = write(fd_2, new_buf, new_size); // writing whole file
    assert(n_write != -1);

    if (n_write == -1) {
        ASSERT_OK(errno)
    }

    close(fd_1);
    close(fd_2);

    return 0;
}


int main(int argc, char** argv) {
    int result = check_input(argc ,argv);
    dump(result);
    handle_input(argc, argv);
    return 0;
}






