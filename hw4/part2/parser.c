#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

char **pipe_spliter(const char *input)
{
    enum { MAX_PIPE_NUM = 512 };

    char **commands = (char **) malloc(sizeof(char *) * MAX_PIPE_NUM);
    char *copied = strdup(input);
    char *cmd = strtok(copied, "|");
    int count = 0;

    do {
        commands[count++] = strdup(cmd);
    } while (!!(cmd = strtok(NULL, "|")));
    free(copied);

    commands[count] = NULL;
    return commands;
}

void free_pipes(char **commands)
{
    FREE_UNTIL_NULL(commands);
}

char **getArgs(const char *command)
{
    enum { ARGV_DEFAULT_SIZE = 512 };
    static const char delim[] = " \t\n";
    while (isspace(*command))
        command++;
    if (!(*command)) {  // is empty line
        char **fail = (char **) malloc(sizeof(char *) << 1);
        fail[0] = (char *) malloc(0);
        fail[1] = NULL;
        return fail;
    }

    char **new_argv = (char **) malloc(sizeof(char *) * ARGV_DEFAULT_SIZE);
    unsigned int new_argv_cap = ARGV_DEFAULT_SIZE;
    unsigned int count = 0;
    char *local_cmd = strdup(command);

    char *tok = strtok(local_cmd, delim);
    do {
        char *new_room = strdup(tok);
        if (__glibc_unlikely(count >= new_argv_cap))  // Double the size
            new_argv = realloc(new_argv, new_argv_cap <<= 1);
        new_argv[count++] = new_room;
    } while (!!(tok = strtok(NULL, delim)) && *tok != '&');
    free(local_cmd);

    new_argv[count] = NULL;
    return new_argv;
}