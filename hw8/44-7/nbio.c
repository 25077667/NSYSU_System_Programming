#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef EXIST
#define FILENAME "./tmp"
#else
#define FILENAME "./foo"
#endif

#define Wunused(expr)   \
    do {                \
        if (!!(expr)) { \
        };              \
    } while (0)

int main(void)
{
    enum { BUF_LEN = 4096 };
    char fifo[BUF_LEN] = {0};
    mkfifo(FILENAME, S_IRUSR | S_IWUSR | S_IWGRP);
    int fd = open(FILENAME, O_RDONLY);

    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);

    Wunused(read(fd, fifo, BUF_LEN));
    for (int i = 0; i < BUF_LEN; i++)
        fputc(fifo[i], stderr);
    return 0;
}