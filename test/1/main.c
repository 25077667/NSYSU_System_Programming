#include <stdio.h>

#ifdef DEBUG
#include <unistd.h>
#else
#include "my_alarm.h"
#endif

int main(void)
{
    puts("foo");
    alarm(2);
    sleep(4);
    puts("bar");
    return 0;
}