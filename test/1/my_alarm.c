#include "my_alarm.h"
#include <string.h>
#include <sys/time.h>
// #define NULL ((void *) 0)

unsigned int my_alarm(unsigned int seconds)
{
    struct itimerval cur, new;
    getitimer(ITIMER_REAL, &cur);
    memcpy(&new, &cur, sizeof(cur));
    new.it_value.tv_sec += seconds;
    return setitimer(ITIMER_REAL, &new, &cur);
}