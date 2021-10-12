/*
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

static inline void _parent_wait(pid_t pid)
{
    int status;
    do {
        pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
        if (w == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        if (WIFSIGNALED(status)) {
            printf("killed by signal %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("stopped by signal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("continued\n");
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

void run_command(char **myArgv)
{
    /*
     *   Rewrite this code.
     */

    const int do_wait = !(is_background(myArgv));
    if (!do_wait) {  // Remove the tail "&"
        int i = -1;
        while (myArgv[++i])
            ;
        free(myArgv[i - 1]);
        myArgv[i - 1] = NULL;
    }

    pid_t pid = fork();
    switch (pid) {
    case 0:  // Child process
        execvp(myArgv[0], myArgv);
        exit(errno);
    case -1:  // Fork failed
        perror("fork");
        exit(errno);
    default:  // Parent process
        if (do_wait)
            _parent_wait(pid);
        break;
    }
}
