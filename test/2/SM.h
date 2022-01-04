#ifndef __SM_H__
#define __SM_H__
#include <stdio.h>
#include <stdlib.h>

struct SM {
    int fd;
    void *ptr;
    unsigned long len;
};

struct SM *new_sm(const char *file, int flag);
// struct SM *new_sm(const char *file, size_t size);
void delete_sm(struct SM *this);
void set_len(struct SM *this, size_t s);

#define error_exit(msg)     \
    do {                    \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

#define handle_error(expr)     \
    do {                       \
        if ((expr) == -1)      \
            error_exit(#expr); \
    } while (0)

#endif