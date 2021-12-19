#ifndef __PRODUCER_H__
#define __PRODUCER_H__
#include <unistd.h>
#include "Broadcast.h"

struct Producer {
    const int message_num;
    const useconds_t sleep_time;
    const unsigned int buffer_num;
    struct Broadcast *const buf;
    void (*const job)(const struct Producer *const);
};

void producer_job(const struct Producer *const this);

#endif