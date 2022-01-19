#ifndef ARG_H
#define ARG_H

struct Arg {
    char *const buf;
    void *const sem;
    _Atomic unsigned int len;
};

#ifndef BUFSIZE
#define BUFSIZE 4096
#endif

#endif