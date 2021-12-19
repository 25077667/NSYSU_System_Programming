#include "Consumer.h"
#if defined(DEBUG_CONSUMER_J) || defined(DEBUG_CONSUMER_RET)
#include <stdio.h>
#endif
#define __glibc_unlikely(cond) __builtin_expect((cond), 0)

int consumer_job(const struct Consumer *const this)
{
    int counter = 0;
    int prev = -1;

    const struct Broadcast *const bc_arr = this->buf;
    for (unsigned int i = 0;;) {
        const int cur = bc_arr[i].data_in;
#ifdef DEBUG_CONSUMER_J
        fprintf(stderr, "%d %d\n", cur, i);
#endif
        if (cur > prev) {
            ++counter;
            prev = cur;
        } else if (cur == -1)
            break;
        ++i;
        if (__glibc_unlikely(i == this->buffer_num))
            i = 0;
    }
#ifdef DEBUG_CONSUMER_RET
    fprintf(stderr, "ret c %d\n", this->work_id);
#endif
    return this->data_num - counter;
}