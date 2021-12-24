/*
 * The same main will do to test all the versions  of  lookup.   The
 * argument  to  main  is  always  passed  as the second argument to
 * lookup. It identifies the resource needed by lookup to  find  its
 * answers.  It  might be a file name or a named pipe, or the key to
 * some shared memory or a message queue, etc. The point is,  it  is
 * just  a  string;  main does not need to understand how the string
 * will be used. The work involved in each  exercise  is  to  define
 * lookup in different ways. What is the same throughout is that the
 * FIRST time lookup is called, it may have to open some resource. A
 * static internal flag is used to determine whether or not this was
 * the first call.
 */

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "dict.h"

static int clientId;

static void __clean_clientId(int sig)
{
    (void) sig;
    close(clientId);
}

static inline void init_sig_handle()
{
    struct sigaction sa = {.sa_flags = 0, .sa_handler = __clean_clientId};
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

int main(int argc, char **argv)
{
    init_sig_handle();
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <resource>\n", argv[0]);
        exit(errno);
    }
    const char *const HOSTNAME = argv[1];

    EE(my_connect(HOSTNAME));
    while (fputs("What word do you want : ", stderr)) {
        Dictrec try_it;
        if (fgets(try_it.word, WORD, stdin) == NULL)
            break;
        try_it.word[strlen(try_it.word) - 1] = 0;  // strip the newline
        (void) lookup(&try_it, HOSTNAME);
        printf("%s : %s", try_it.word, try_it.text);
    }
    EE(my_disconnect());
    return 0;
}
