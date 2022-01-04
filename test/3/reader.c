#include "reader.h"
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include "arg.h"

void *reader(void *arg__)
{
    struct Arg *const arg = (struct Arg *) arg__;
    char my_buf[BUFSIZE] = {0};
    ssize_t len = 0;
    while ((len = read(STDIN_FILENO, my_buf, BUFSIZE)) != 0) {
        int is_busy = 0;
        sem_getvalue(arg->sem, &is_busy);
        if (is_busy)
            sem_wait(arg->sem);
        if (len != BUFSIZE)
            memset(arg->buf + len, -1, BUFSIZE - len);
        memcpy(arg->buf, my_buf, len);
        sem_post(arg->sem);
    }
    return NULL;
}