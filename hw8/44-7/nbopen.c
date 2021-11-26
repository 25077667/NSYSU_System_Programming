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
    char buf[BUF_LEN] = {0};
    int fd = open(FILENAME, O_RDONLY | O_NONBLOCK);

    Wunused(read(fd, buf, BUF_LEN));
    for (int i = 0; i < BUF_LEN; i++)
        fputc(buf[i], stderr);
    return 0;
}