#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

static inline void __redirect_to(char **cmd, char mode)
{
    const int IO = (mode >> 1) & 1;
    static const char *rw[] = {"r", "w"};
    int i = 0;
    char *tok = NULL;
    do {
        tok = cmd[i++];
        if (tok == NULL)
            return;
    } while (*tok != mode);

    if (!cmd[i])  // Not found redirection
        return;

    // cmd[i] is the filename
    FILE *f = fopen(cmd[i], rw[IO]);
    dup2(fileno(f), IO);
    int j = i;
    // Find the end index (j)
    while (cmd[++j] != NULL)
        ;

    // "Erase" redirect (from/to) symbol and name
    // Concatnate the tail options
    char *trash1 = cmd[i - 1], *trash2 = cmd[i];
    memcpy(cmd[i - 1], cmd[i + 1], j - i);
    free(trash1);
    free(trash2);
}

void redirect(char **cmd)
{
    __redirect_to(cmd, '<');
    __redirect_to(cmd, '>');
}