#include "Producer.h"
#include <stdio.h>

static inline void write_msg(struct Broadcast *bc, int i)
{
    snprintf(bc->buffer, 256, "This is message %d\n", i);
#ifdef DEBUG_PRODUCER
    fprintf(stderr, "%p %d\n", bc, i);
#endif
    bc->data_in = i;
}

static inline void delay(useconds_t us)
{
    usleep(us * 1000);
}

void producer_job(struct Producer *this)
{
    struct Broadcast *const bc_arr = this->buf;
    for (int i = 0; i < this->message_num; i++) {
        delay(this->sleep_time);
        write_msg(&bc_arr[i % this->buffer_num], i);
    }
    // Final chunk of Broadcast
    write_msg(&bc_arr[this->message_num % this->buffer_num], -1);
}