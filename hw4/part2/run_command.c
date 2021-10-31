/*
 * run_command.c :    do the fork, exec stuff, call other functions
 */

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

static inline size_t cmd_num(const char **cmd)
{
    size_t num = 0;
    while (*cmd++)
        ++num;
    return num;
}

void run_command(char **commands)
{
    size_t index = cmd_num((const char **) commands);
    int fd[2], status;
    pid_t pid;
    /* Fork a child to execute commands */
    if (!!(pid = fork())) { /* This is parent(aka. ./myshell) */
        void *notWait = strchr(commands[index - 1], '&');
        while (!notWait && waitpid(pid, &status, WUNTRACED | WCONTINUED) &&
               !WIFEXITED(status) && !WIFSIGNALED(status))
            ;
        if (notWait)
            printf("[%d]\n", pid);
        return;
    }
    /* All commands would be executed here
     *
     * For example:
     * $ ls | cat | less
     * Works like:
     *
     * .|   out          in   out          in   out          in
     * /|     ____________      ____________      ____________
     * m|    |            |    |            |    |            |
     * y|    |            |    |            |    |            |
     * s|----|    less    |--->|     cat    |--->|     ls     |
     * h|    |            |    |            |    |            |
     * e|    |____________|    |____________|    |____________|
     * l|              ------------>     ------------>
     * l|                  fork              fork
     *
     *
     * Child's stdout to parent's stdin.
     *
     * And afterwards, kill all forked childs(commands).
     */
    while (--index) {
        /* set all all pipe line */
        if (pipe(fd) < 0)
            handle_error("pipe");
        pid = fork();
        if (pid > 0) {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            /* Last be piped process will leave first */
            break;
        } else if (pid == 0) {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
        } else
            handle_error("fork");
    }

    char **arg = getArgs(commands[index]);
    redirect(arg);
    if (execvp(arg[0], arg) < 0)
        puts("Command not found!");

    fflush(NULL);  // flush all
    FREE_UNTIL_NULL(arg);
    /* Kill all child process */
    exit(errno);
}
