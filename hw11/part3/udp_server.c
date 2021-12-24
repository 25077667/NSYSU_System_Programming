/*
 * udp_server : listen on a UDP socket ;reply immediately
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "dict.h"

static int serverId;

static void __clean_serverId(int sig)
{
    (void) sig;
    close(serverId);
    exit(0);
}

static inline void serve_request(const struct sockaddr_in *const from,
                                 const char *const msg)
{
    Dictrec try_it = {.word = {0}, .text = "Not Found!\n"};
    memcpy(try_it.word, msg, WORD);
    (void) lookup(&try_it, NULL);

    ssize_t size = sendto(serverId, try_it.text, sizeof(try_it.text), 0,
                          (const struct sockaddr *) from, sizeof(*from));
    printf("%ld\n", size);
    perror("sendto");
}

static inline void init_sig_handle()
{
    struct sigaction sa = {.sa_flags = 0, .sa_handler = __clean_serverId};
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

int main(int argc, char **argv)
{
    init_sig_handle();
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <dictionary source>\n", argv[0]);
        exit(errno);
    }

    struct sockaddr_in server = {.sin_addr = {0},
                                 .sin_family = AF_INET,
                                 .sin_port = htons(PORT),
                                 .sin_zero = {0}};
    const int sd = socket(AF_INET, SOCK_DGRAM, 0);
    const socklen_t ser_len = sizeof(server);
    EE(bind(sd, (struct sockaddr *) &server, ser_len));

    serverId = sd;            // Copy local para to global
    EE(my_connect(argv[1]));  // Connect the database first

    while (1) {
        struct sockaddr_in from = {.sin_addr = {0},
                                   .sin_family = AF_INET,
                                   .sin_port = 0,
                                   .sin_zero = {0}};
        socklen_t len = sizeof(from);
        char buf[WORD] = {0};
        if (recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *) &from,
                     &len)) {
            serve_request(&from, buf);
        }
    }
    EE(my_disconnect());
    __clean_serverId(0);
    return 0;
}
