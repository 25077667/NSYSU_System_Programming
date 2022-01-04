#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include "arg.h"
#include "reader.h"
#include "writer.h"

static char glo_buf[BUFSIZE];
#define SEM_NAME "SCC"

int main(void)
{
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_RDWR);
    sem_init(sem, 0, 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    struct Arg arg = {
        .buf = glo_buf,
        .sem = (void *const) sem,
    };

    pthread_t consumer, producer;
    pthread_create(&consumer, &attr, writer, (void *) &arg);
    pthread_create(&producer, &attr, reader, (void *) &arg);
    void *res;
    pthread_join(producer, &res);
    pthread_join(consumer, &res);

    sem_close(sem);
    sem_unlink(SEM_NAME);

    return 0;
}