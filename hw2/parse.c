/*
 * parse.c : use whitespace to tokenise a line
 * Initialise a vector big enough
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char **parse(char *line)
{
    static const char delim[] = " \t\n"; /* SPACE or TAB or NL */
    unsigned int count = 0;
    char *token = NULL;
    char **new_argv = NULL;
    enum { ARGV_DEFAULT_SIZE = 256 };

    /* Nothing entered. */
    if (line == NULL) {
        return NULL;
    }

    /* Init strtok with commandline, then get first token.
     * Return NULL if no tokens in line.
     *
     * Fill in code.
     */
    token = strtok(line, delim);

    /* Create an array with room for first token.
     *
     * Fill in code.
     */

    new_argv = (char **) malloc(sizeof(char *) * ARGV_DEFAULT_SIZE);
    static unsigned int new_argv_cap = ARGV_DEFAULT_SIZE;
    size_t tok_len = strlen(token);
    char *room = (char *) malloc(tok_len + 1);
    strncpy(room, token, tok_len);
    room[tok_len] = '\0';
    new_argv[count++] = room;
    printf("[%d] : %s\n", count - 1, new_argv[count - 1]);


    /* While there are more tokens...
     *
     *  - Get next token.
     *	- Resize array.
     *  - Give token its own memory, then install it.
     *
     * Fill in code.
     */
    while (!!(token = strtok(NULL, delim))) {
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
    }


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
