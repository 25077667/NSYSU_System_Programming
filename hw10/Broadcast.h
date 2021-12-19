#ifndef __BROADCAST_H__
#define __BROADCAST_H__

#include <stdatomic.h>

struct Broadcast {
    atomic_int data_in;
    char buffer[256];
};

#endif