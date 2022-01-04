#define _GNU_SOURCE
#include "SM.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

struct SM *new_sm(const char *filename, int flag)
{
    const int fd = open(filename, flag, 0777);
    struct stat sb;
    handle_error(fstat(fd, &sb));

    struct SM *ret = (struct SM *) malloc(sizeof(struct SM));
    ret->fd = fd;
    ret->len = sb.st_size;
    ret->ptr = (ret->len) ? mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE,
                                 MAP_SHARED, fd, 0)
                          : NULL;
    return ret;
}

void delete_sm(struct SM *this)
{
    if (this && this->ptr) {
        close(this->fd);
        munmap(this->ptr, this->len);
    }
    free(this);
}

void set_len(struct SM *this, size_t s)
{
    ftruncate(this->fd, s);
    if (!this->ptr)
        this->ptr =
            (s) ? mmap(NULL, s, PROT_READ | PROT_WRITE, MAP_SHARED, this->fd, 0)
                : NULL;
    else
        this->ptr =
            (s) ? mremap(this->ptr, this->len, s, MREMAP_MAYMOVE) : NULL;
    this->len = s;
}