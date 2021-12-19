#include "Consumer.h"
#include <stdio.h>
static int counter = 0;
static int prev = -1;

int consumer_job(struct Consumer *this)
{
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