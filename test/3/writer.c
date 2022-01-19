#include "writer.h"
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include "arg.h"

void *writer(void *arg__)
{
    struct Arg *const arg = (struct Arg *) arg__;
    char my_buf[BUFSIZE] = {0};
    do {
        ssize_t len = 0;
        int is_ready = 0;
        sem_getvalue(arg->sem, &is_ready);
        if (!is_ready)
            sem_wait(arg->sem);
        len = arg->len;
        memcpy(my_buf, arg->buf, len);
        write(STDOUT_FILENO, my_buf, len);
    } while (arg->len);

    return NULL;
}