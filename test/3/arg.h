#ifndef ARG_H
#define ARG_H

// The buf cannot contains -1 (0xff)
struct Arg {
    char *const buf;
    void *const sem;
};

#ifndef BUFSIZE
#define BUFSIZE 4096
#endif

#endif