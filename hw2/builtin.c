/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/


/* "echo" command.  Do not print final <CR> if "-n" encountered. */
static void bi_echo(char **argv)
{
    int i = 0;
    if (argv[1] && __glibc_unlikely(strcmp(argv[1], "-n") == 0)) {
        const int n_value = atoi(argv[2]);
        puts(argv[2 + n_value]);
    } else {
        while (argv[i] && argv[++i])
            printf("%s\n", argv[i]);
    }
    /* Fill in code. */
}

/* Fill in code. */
#define __DECLARE_bi(func)                     \
    static inline void _bi_##func(char **argv) \
    {                                          \
        free_argv(argv);                       \
        exit(0);                               \
    }

__DECLARE_bi(logout);
__DECLARE_bi(bye);
__DECLARE_bi(exit);
__DECLARE_bi(quit);

static void _bi_chdir(char **argv)
{
    static char cwd[4096];
    const unsigned int path_len = strlen(getcwd(cwd, sizeof(cwd)));
    const char *arg = argv[1];
    if (arg[0] == '.')
        strncat(cwd + path_len, arg, strlen(arg));

    chdir(arg);
}

/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
    char *keyword;          /* When this field is argv[0] ... */
    void (*do_it)(char **); /* ... this function is executed. */
} inbuilts[7] = {
    /* Fill in code. */
    {"bye", _bi_bye},   {"logout", _bi_logout}, {"exit", _bi_exit},
    {"quit", _bi_quit}, {"echo", bi_echo},      {"cd", _bi_chdir},
    {NULL, NULL} /* NULL terminated. */
};



/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd *this; /* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the inbuilts table above.
Hold handle to it if it is. */
int is_builtin(char *cmd)
{
    struct cmd *tableCommand;

    for (tableCommand = inbuilts; tableCommand->keyword != NULL; tableCommand++)
        if (strcmp(tableCommand->keyword, cmd) == 0) {
            this = tableCommand;
            return TRUE;
        }
    return FALSE;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv)
{
    this->do_it(argv);
    return 0;
}
