/*
 * lookup8 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet TCP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "dict.h"

static int ser_fd;

int my_connect(const char *name)
{
    const struct hostent *hp = gethostbyname(name);
    ser_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in host = {.sin_family = AF_INET,
                               .sin_port = htons(PORT),
                               .sin_addr = {*((unsigned long *) hp->h_addr)},
                               .sin_zero = {0}};
    return connect(ser_fd, (const struct sockaddr *) &host, sizeof(host));
}

int lookup(Dictrec *sought, const char *resource)
{
    (void) resource;
    Wunused(write(ser_fd, sought->word, strlen(sought->word) + 1));
    return read(ser_fd, sought->text, sizeof(sought->text));
}

int my_disconnect(void)
{
    close(ser_fd);
    return 0;
}