/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char **parse(char *line)
{
    static const char delim[] = " \t\n";
    enum { ARGV_DEFAULT_SIZE = 256 };

    /* Strip spaces */
    while (isspace(*line))
        line++;
    if ((*line) == 0)
        return NULL;

    char *token = strtok(line, delim);
    char **new_argv = (char **) malloc(sizeof(char *) * ARGV_DEFAULT_SIZE);
    unsigned int new_argv_cap = ARGV_DEFAULT_SIZE;

    unsigned int count = 0;
    do {
        size_t tok_len = strlen(token);
        char *new_room = (char *) malloc(tok_len + 1);
        strncpy(new_room, token, tok_len);
        new_room[tok_len] = '\0';
        if (__glibc_unlikely(count >= new_argv_cap)) {
            new_argv =
                realloc(new_argv, new_argv_cap <<= 1);  // Double the size
        }
        new_argv[count++] = new_room;
        printf("[%d] : %s\n", count - 1, new_argv[count - 1]);
    } while (!!(token = strtok(NULL, delim)));

    new_argv[count] = NULL;
    return new_argv;
}


/*
 * Free memory associated with argv array passed in.
 * Argv array is assumed created with parse() above.
 */
void free_argv(char **oldArgv)
{
    unsigned int i = 0;
    while (oldArgv[i])
        free(oldArgv[i++]);

    free(oldArgv);
}
