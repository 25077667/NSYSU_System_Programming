#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define Wunused(expr) \
    do {              \
        if ((expr)) { \
        };            \
    } while (0)

void hostinfo(void)
{
    fprintf(stdout, "hostname: ");
    fflush(stdout);
    Wunused(system("hostname;uname -r; echo -n \"hostid: \"; hostid"));
}

void mydate(void)
{
    Wunused(system("date \"+%b %d(%a), %Y %l:%M %p\""));
}

void printdir(void)
{
    char buf[256];
    printf("%s\n", getcwd(buf, sizeof(buf)));
}

void mycat(const char *arg)
{
    size_t len = strlen(arg);
    char buf[len + 5];
    snprintf(buf, len + 5, "cat %s", arg);
    Wunused(system(buf));
}

void pipe_ls(void)
{
    int fd[2];
    Wunused(pipe(fd));

    if (fork()) {
        char c;
        close(fd[1]);
        while (!!(read(fd[0], &c, 1)))
            Wunused(write(STDOUT_FILENO, &c, 1));
        close(fd[0]);
    } else {
        char *argv[] = {"ls", "-al", NULL};
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        execvp(argv[0], argv);
        exit(0);
    }
}