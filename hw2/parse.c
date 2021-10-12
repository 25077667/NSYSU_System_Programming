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
    static const char delim[] = " \t\n"; /* SPACE or TAB or NL */
    unsigned int count = 0;
    enum { ARGV_DEFAULT_SIZE = 256 };

    /* Strip spaces */
    while (isspace(*line))
        line++;

    if ((*line) == 0)
        return NULL;
    /* Init strtok with commandline, then get first token.
     * Return NULL if no tokens in line.
     *
     * Fill in code.
     */
    char *token = strtok(line, delim);
    if (!token)
        return NULL;

    /* Create an array with room for first token.
     *
     * Fill in code.
     */

    char **new_argv = (char **) malloc(sizeof(char *) * ARGV_DEFAULT_SIZE);
    static unsigned int new_argv_cap = ARGV_DEFAULT_SIZE;

    /* While there are more tokens...
     *
     *  - Get next token.
     *	- Resize array.
     *  - Give token its own memory, then install it.
     *
     * Fill in code.
     */
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


    /* Null terminate the array and return it.
     *
     * Fill in code.
     */
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

    /* Free each string hanging off the array.
     * Free the oldArgv array itself.
     *
     * Fill in code.
     */
    while (oldArgv[i])
        free(oldArgv[i++]);

    free(oldArgv);
}
