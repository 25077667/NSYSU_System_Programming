#ifndef MY_ALARM_H
#define MY_ALARM_H
#include <unistd.h>

unsigned int my_alarm(unsigned int seconds);

#define alarm my_alarm
#endif