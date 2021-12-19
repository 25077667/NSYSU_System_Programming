#ifndef __COMSUMER_H__
#define __COMSUMER_H__
#include "Broadcast.h"

struct Consumer {
    const unsigned int work_id;
    const unsigned int data_num;
    const unsigned int buffer_num;
    struct Broadcast *const buf;
    int (*const job)(const struct Consumer *const);  // Return number of failed
};

// Return number of failed
int consumer_job(const struct Consumer *const this);

#endif