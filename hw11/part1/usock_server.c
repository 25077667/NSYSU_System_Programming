/*
 * usock_server : listen on a Unix socket ; fork ;
 *                child does lookup ; replies down same socket
 * argv[1] is the name of the local datafile
 * PORT is defined in dict.h
 */

#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include "dict.h"

static int serverId;
static char sun_path[108];

static void __clean_serverId(int sig)
{
    (void) sig;
    close(serverId);
    unlink(sun_path);
}

static inline void serve_request(int cli_fd, const char *dict_name)
{
    char buf[256] = {0};
    EE(my_connect(dict_name));  // Connect the database first
    while (read(cli_fd, buf, 256) != EOF) {
        Dictrec try_it = {.word = {0}, .text = "Not Found!\n"};
        strcpy(try_it.word, buf);
        puts(buf);
        (void) lookup(&try_it, dict_name);
        Wunused(write(cli_fd, try_it.text, strlen(try_it.text) + 1));
        memset(buf, 0, 256);
    }

    EE(my_disconnect());
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
    if (argc != 3) {
        fprintf(stderr,
                "Usage : %s <dictionary source> "
                "<Socket name>\n",
                argv[0]);
        exit(errno);
    }

    struct sockaddr_un server = {.sun_family = AF_UNIX, .sun_path = {0}};
    strcpy(server.sun_path, argv[2]);
    const int sd = socket(AF_UNIX, SOCK_STREAM, 0);
    const socklen_t ser_len =
        strlen(server.sun_path) + sizeof(server.sun_family);
    EE(bind(sd, (struct sockaddr *) &server, ser_len));
    EE(listen(sd, 8));

    // Copy local para to global
    strncpy(sun_path, server.sun_path, sizeof(sun_path));
    serverId = sd;

    while (1) {
        socklen_t ser_len = sizeof(server.sun_family) + strlen(server.sun_path);
        const int cd = accept(sd, (struct sockaddr *) &server, &ser_len);
        if (cd == -1)
            break;
        /* Handle new client in a subprocess. */
        if (!fork()) {
            close(sd); /* Rendezvous socket is for parent only. */
            serve_request(cd, argv[1]);
            exit(0);
        }
        close(cd);
    }

    __clean_serverId(0);
    return 0;
}
