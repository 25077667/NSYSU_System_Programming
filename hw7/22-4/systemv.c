#define _XOPEN_SOURCE 500
#include "systemv.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

thread_local struct sigaction sa;  // C11

sighandler_t my_sigset(int sig, sighandler_t disp)
{
    if (disp == SIG_IGN)
        sigaddset(&sa.sa_mask, sig);
    else if (disp == SIG_HOLD)
        sigaddset(&sa.sa_mask, sig);
    else {  // Included SIG_DFL
        sigdelset(&sa.sa_mask, sig);
        memcpy(&sa.__sigaction_handler, &disp, sizeof(disp));
        sigaction(sig, &sa, &sa.__sigaction_handler);
    }

    return (errno != 0) ? SIG_HOLD : (sighandler_t) -1;
}

int my_sighold(int sig)
{
    return sigaddset(&sa.sa_mask, sig);
}

int my_sigrelse(int sig)
{
    return sigdelset(&sa.sa_mask, sig);
}

int my_sigignore(int sig)
{
    // sa.__sigaction_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    return sigaction(sig, &sa, NULL);
}

int my_sigpause(int sig)
{
    sigdelset(&sa.sa_mask, sig);
    return sigsuspend(&sa.sa_mask);
}