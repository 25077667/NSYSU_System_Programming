/*
 * lookup9 : does no looking up locally, but instead asks
 * a server for the answer. Communication is by Internet UDP Sockets
 * The name of the server is passed as resource. PORT is defined in dict.h
 */

#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "dict.h"

// Do nothing
int my_connect(const char *name)
{
    (void) name;
    return 0;
}

// Using UDP to ask for sought
int lookup(Dictrec *sought, const char *const resource)
{
    const struct hostent *hp = gethostbyname(resource);
    struct sockaddr_in server = {.sin_addr = {*((unsigned long *) hp->h_addr)},
                                 .sin_family = AF_INET,
                                 .sin_port = htons(PORT),
                                 .sin_zero = {0}};
    socklen_t len = sizeof(server);
    const int cli_fd = socket(AF_INET, SOCK_DGRAM, 0);
    sendto(cli_fd, sought->word, sizeof(sought->word), 0,
           (const struct sockaddr *) &server, len);

    return recvfrom(cli_fd, sought->text, sizeof(sought->text), 0,
                    (struct sockaddr *) &server, &len);
}

// Do nothing
int my_disconnect(void)
{
    return 0;
}