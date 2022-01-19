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
    int is_busy = 0;
    while ((len = read(STDIN_FILENO, my_buf, BUFSIZE)) != 0) {
        sem_getvalue(arg->sem, &is_busy);
        if (is_busy)
            sem_wait(arg->sem);
        memcpy(arg->buf, my_buf, len);
        arg->len = len;
        sem_post(arg->sem);
    }
    sem_getvalue(arg->sem, &is_busy);
    if (is_busy)
        sem_wait(arg->sem);
    arg->len = 0;
    sem_post(arg->sem);
    return NULL;
}