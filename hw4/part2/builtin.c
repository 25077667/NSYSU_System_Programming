/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
 */

#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/
static void bi_builtin(char **argv); /* "builtin" command tells whether a
                                        command is builtin or not. */
static void bi_cd(char **argv);      /* "cd" command. */
static void bi_echo(char **argv); /* "echo" command.  Does not print final <CR>
                                     if "-n" encountered. */
static void bi_hostname(char **argv); /* "hostname" command. */
static void bi_id(
    char **argv); /* "id" command shows user and group of this process. */
static void bi_pwd(char **argv);  /* "pwd" command. */
static void bi_quit(char **argv); /* quit/exit/logout/bye command. */


/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
    char *keyword;          /* When this field is argv[0] ... */
    void (*do_it)(char **); /* ... this function is executed. */
} inbuilts[] = {
    {"builtin", bi_builtin}, /* List of (argv[0], function) pairs. */

    /* Fill in code. */
    {"echo", bi_echo},
    {"quit", bi_quit},
    {"exit", bi_quit},
    {"bye", bi_quit},
    {"logout", bi_quit},
    {"cd", bi_cd},
    {"pwd", bi_pwd},
    {"id", bi_id},
    {"hostname", bi_hostname},
    {NULL, NULL} /* NULL terminated. */
};


static void bi_builtin(char **argv)
{
    const char *real_cmd = argv[1];
    static const char *NOTornot[2] = {"", " NOT"};
    printf("%s is%s a builtin feature.\n", real_cmd,
           NOTornot[!is_builtin(real_cmd)]);
}

static void bi_cd(char **argv)
{
    static char cwd[4096];
    const unsigned int path_len = strlen(getcwd(cwd, sizeof(cwd)));
    const char *arg = argv[1];
    if (arg[0] == '.')
        strncat(cwd + path_len, arg, strlen(arg));

    chdir(arg);
}

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
}

static void bi_hostname(char **argv)
{
    char name[256] = {0};
    gethostname(name, 256);
    printf("hostname: %s\n", name);
}

static void bi_id(char **argv)
{
    const unsigned int uid = getuid();
    struct passwd *pws;
    pws = getpwuid(uid);
    const unsigned int gid = pws->pw_gid;

    printf("UserID = %d(%s), GroupID = %d(%s)\n", uid, pws->pw_name, gid,
           getgrgid(gid)->gr_name);
}

static void bi_pwd(char **argv)
{
    char cwd[4096] = {0};
    strlen(getcwd(cwd, sizeof(cwd)));
    printf("%s\n", cwd);
}

static void bi_quit(char **argv)
{
    exit(0);
}


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
            return 1;
        }
    return 0;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv)
{
    this->do_it(argv);
}
