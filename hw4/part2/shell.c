/*
 * shell.c  : test harness for parse routine
 */

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *inputString()
{
    size_t len = 0, size = 64;
    char *str = realloc(NULL, size), ch;
    if (!str)
        return NULL;
    while ((ch = fgetc(stdin)) != EOF && ch != '\n' && ch != '\r') {
        str[len++] = ch;
        if (len == size && !(str = realloc(str, (size += 64))))
            return NULL;
    }
    str[len++] = 0;
    /* If it's empty string free it and return NULL, else return the str */
    return (strlen(str) ? str : (free(str), NULL));
}

int main(void)
{
    while (1) {
        fputs("myshell -> ", stdout);
        char *line = inputString();
        if (!line)
            continue;

        char **cmd = pipe_spliter(line);
        free(line);

        if (is_builtin((const char **) cmd)) {
            do_builtin((const char **) cmd);
            free_pipes(cmd);
            continue;
        }
        run_command(cmd);
        free_pipes(cmd);
    }
    return 0;
}
