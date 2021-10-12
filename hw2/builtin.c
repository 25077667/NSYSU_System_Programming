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
#include "shell.h"

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/

/* Fill in code. */
#define __DECLARE_bi(func) \
    static inline void _bi_##func(const char **argv) { exit(0); }

__DECLARE_bi(logout);
__DECLARE_bi(bye);
__DECLARE_bi(exit);
__DECLARE_bi(quit);
__DECLARE_bi(echo);


/* "echo" command.  Do not print final <CR> if "-n" encountered. */
static void bi_echo(const char **argv)
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
/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
    char *keyword;                /* When this field is argv[0] ... */
    void (*do_it)(const char **); /* ... this function is executed. */
} inbuilts[6] = {
    /* Fill in code. */
    {"bye", _bi_bye},   {"logout", _bi_logout},
    {"exit", _bi_exit}, {"quit", _bi_quit},
    {"echo", bi_echo},  {NULL, NULL} /* NULL terminated. */
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
    this->do_it((const char **) argv);
    return 0;
}
