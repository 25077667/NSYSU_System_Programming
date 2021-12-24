/*
 * lookup7 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Unix TCP Sockets
 * The name of the socket is passed as resource.
 */

#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "dict.h"

static const struct sockaddr_un *ser_addr;
static const int *ser_fd;

//! Dirty here but this is the only way to ctor const shared global data.
static inline void set_addr_(const char *name)
{
    struct sockaddr_un sk = {.sun_family = AF_UNIX, .sun_path = {0}};
    strcpy(sk.sun_path, name);
    ser_addr = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));
    memcpy((struct sockaddr_un *) ser_addr, &sk, sizeof(sk));

    int sd = socket(AF_UNIX, SOCK_STREAM, 0);
    ser_fd = (const int *) malloc(sizeof(int));
    *(int *) ser_fd = sd;
}

int my_connect(const char *name)
{
    set_addr_(name);
    const socklen_t len =
        strlen(ser_addr->sun_path) + sizeof(ser_addr->sun_family);
    return connect(*ser_fd, (const struct sockaddr *) ser_addr, len);
}

int lookup(Dictrec *sought, const char *resource)
{
    (void) resource;
    Wunused(write(*ser_fd, sought->word, strlen(sought->word) + 1));
    return read(*ser_fd, sought->text, sizeof(sought->text));
}

int my_disconnect(void)
{
    close(*ser_fd);
    free((void *) ser_fd);
    free((void *) ser_addr);
    return 0;
}