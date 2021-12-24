
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
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <dictionary source>\n", argv[0]);
        exit(errno);
    }

    struct sockaddr_in server = {.sin_addr = {0},
                                 .sin_family = AF_INET,
                                 .sin_port = htons(PORT),
                                 .sin_zero = {0}};
    const int sd = socket(AF_INET, SOCK_STREAM, 0);
    const socklen_t ser_len = sizeof(server);
    EE(bind(sd, (struct sockaddr *) &server, ser_len));
    EE(listen(sd, 8));

    // Copy local para to global
    serverId = sd;

    while (1) {
        socklen_t ser_len = sizeof(server);
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
