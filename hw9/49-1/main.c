#include <fcntl.h>   // OPEN
#include <stdlib.h>  // error
#include <string.h>
#include <unistd.h>
#include "SM.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "%s src dst\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct SM *src = new_sm(argv[1], O_RDWR);
    struct SM *dst = new_sm(argv[2], O_CREAT | O_RDWR);

    set_len(dst, src->len);

    memcpy(dst->ptr, src->ptr, src->len);

    delete_sm(src);
    delete_sm(dst);

    return 0;
}