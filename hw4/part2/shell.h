/* shell.h : function prototypes for all functions in shell exercise */
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>

#define handle_error(s) \
    do {                \
        perror((s));    \
        exit(1);        \
    } while (0)

#define FREE_UNTIL_NULL(ptr) \
    do {                     \
        unsigned int i = 0;  \
        while (ptr[i])       \
            free(ptr[i++]);  \
        free(ptr);           \
    } while (0)


/* do the fork, exec stuff */
void run_command(char **commands);

/*
 * A Parser that separate commands into 2D-array by pipe line
 */
char **pipe_spliter(const char *input);
void free_pipes(char **commands);

// Redirect stdio and files
void redirect(char **cmd);

/* Get a command's arguments, you should free it by yourself.
 * The terminal element is a NULL.
 */
char **getArgs(const char *command);

int is_builtin(const char **cmd);
void do_builtin(const char **argv);
#endif