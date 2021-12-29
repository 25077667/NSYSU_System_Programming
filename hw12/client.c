#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define DRV_DEV "/dev/demo"
#define DIE(str) perror(str)
#define EE(expr)                       \
    do {                               \
        __auto_type val = (expr);      \
        if (val == (typeof(val)) (-1)) \
            DIE(#expr);                \
    } while (0)

int main(void)
{
    char buf[256] = {0};
    const int fd = open(DRV_DEV, O_RDWR);
    read(fd, buf, 256);
    printf("%s", buf);
    EE(write(fd, "foo", sizeof("foo")));
    close(fd);
    return 0;
}