#ifndef __COMSUMER_H__
#define __COMSUMER_H__
#include "Broadcast.h"

struct Consumer {
    unsigned int work_id;
    unsigned int data_num;
    unsigned int buffer_num;
    struct Broadcast *buf;
    int (*job)(struct Consumer *);  // Return number of failed
};

// Return number of failed
int consumer_job(struct Consumer *const this);

#endif