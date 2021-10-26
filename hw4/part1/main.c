
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

static inline void fail_cat(void)
{
    fprintf(stderr, "Usage: mycat filename\n");
    exit(1);
}

static inline void this_is_a_busybox(void)
{
    puts("This is a busybox.");
}

static struct cmd {
    char *name;
    void (*do_it)(void);
} busybox[6] = {
    {"hostinfo", hostinfo}, {"mydate", mydate},  {"printdir", printdir},
    {"pipe_ls", pipe_ls},   {"mycat", fail_cat}, {"busybox", this_is_a_busybox},
};

static inline void do_busybox(char *name)
{
    const char *base = basename(name);
    for (int i = 0; i < 6; ++i)
        if (!(strcmp(base, busybox[i].name))) {
            busybox[i].do_it();
            break;
        }
}

int main(int argc, char *argv[])
{
    if (argc == 1)
        do_busybox(argv[0]);
    else if (argc == 2)
        mycat(argv[1]);
    else
        fail_cat();

    return 0;
}