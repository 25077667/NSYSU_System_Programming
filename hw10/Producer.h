#ifndef __PRODUCER_H__
#define __PRODUCER_H__
#include <unistd.h>
#include "Broadcast.h"

struct Producer {
    int message_num;
    useconds_t sleep_time;
    unsigned int buffer_num;
    struct Broadcast *buf;
    void (*job)(struct Producer *);
};

void producer_job(struct Producer *const this);

#endif