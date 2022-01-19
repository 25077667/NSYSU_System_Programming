#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/stat.h>
#include "arg.h"
#include "reader.h"
#include "writer.h"

static char glo_buf[BUFSIZE];
#define SEM_NAME "SCC"

// Hope the scheduler will treat these threas farely.
int main(void)
{
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem");
        sem_close(sem);
        sem_unlink(SEM_NAME);
        return -1;
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    struct Arg arg = {
        .buf = glo_buf,
        .sem = (void *const) sem,
        .len = -1,
    };

    pthread_t consumer, producer;
    pthread_create(&consumer, &attr, writer, (void *) &arg);
    pthread_create(&producer, &attr, reader, (void *) &arg);
    void *res = NULL;
    pthread_join(producer, &res);
    pthread_join(consumer, &res);

    sem_close(sem);
    sem_unlink(SEM_NAME);

    return 0;
}