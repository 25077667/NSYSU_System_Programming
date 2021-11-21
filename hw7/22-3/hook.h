#ifndef __HOOK_H__
#define __HOOK_H__
#include <signal.h>

#define sigsuspend my_sigwaitinfo

int my_sigwaitinfo(const sigset_t *__set);

#endif