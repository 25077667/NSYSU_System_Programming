#include "writer.h"
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include "arg.h"

static inline int check_end(const char *buf, int len)
{
    for (int i = -1; i < len;)
        if (__glibc_unlikely(buf[++i] == (char) -1))
            return i;
    return BUFSIZE;
}

void *writer(void *arg__)
{
    struct Arg *arg = (struct Arg *) arg__;
    char my_buf[BUFSIZE] = {0};

    do {
        int is_ready = 0;
        sem_getvalue(arg->sem, &is_ready);
        if (!is_ready)
            sem_wait(arg->sem);
        memcpy(my_buf, arg->buf, BUFSIZE);
        const int len = check_end(my_buf, BUFSIZE);
        write(STDOUT_FILENO, my_buf, len);
        if (len != BUFSIZE)
            break;
    } while (1);

    return NULL;
}