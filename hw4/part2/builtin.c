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
#include <string.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/
static void bi_builtin(const char **argv);  /* "builtin" command tells
                                         whether a command is builtin or not. */
static void bi_cd(const char **argv);       /* "cd" command. */
static void bi_echo(const char **argv);     /* "echo" command.  Does not print
                                        final <CR> if "-n" encountered. */
static void bi_hostname(const char **argv); /* "hostname" command. */
static void bi_id(const char **argv);       /* "id" command shows user and group
                                                    of this process. */
static void bi_pwd(const char **argv);      /* "pwd" command. */
static void bi_quit(const char **argv);     /* quit/exit/logout/bye command. */


/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
    char *keyword;                /* When this field is argv[0] ... */
    void (*do_it)(const char **); /* ... this function is executed. */
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

// Guarantee the argv is formatted in "builtin %s"
static void bi_builtin(const char **argv)
{
    static const char *NOTornot[2] = {"", " NOT"};
    printf("%s is%s a builtin feature.\n", argv[1],
           NOTornot[!is_builtin((const char **) argv + 1)]);
}

static void bi_cd(const char **argv)
{
    static char cwd[4096];
    const unsigned int path_len = strlen(getcwd(cwd, sizeof(cwd)));
    const char *const arg = argv[1];
    if (arg[0] == '.')
        strncat(cwd + path_len, arg, strlen(arg));

    if (!!chdir(arg)) {
    };
    return;
}

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
}

static void bi_hostname(const char **argv)
{
    (void) argv;
    char name[256] = {0};
    gethostname(name, 256);
    printf("hostname: %s\n", name);
}

static void bi_id(const char **argv)
{
    (void) argv;
    const unsigned int uid = getuid();
    struct passwd *pws;
    pws = getpwuid(uid);
    const unsigned int gid = pws->pw_gid;

    printf("UserID = %d(%s), GroupID = %d(%s)\n", uid, pws->pw_name, gid,
           getgrgid(gid)->gr_name);
}

static void bi_pwd(const char **argv)
{
    (void) argv;
    char cwd[4096] = {0};
    strlen(getcwd(cwd, sizeof(cwd)));
    printf("%s\n", cwd);
}

static void bi_quit(const char **argv)
{
    FREE_UNTIL_NULL((char **) argv);
    exit(0);
}


/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd *this; /* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the builtins table above.
Hold handle to it if it is. */
int is_builtin(const char **cmd)
{
    char **cmds = getArgs(cmd[0]);
    for (struct cmd *toc = inbuilts; toc->keyword != NULL; toc++)
        if (strcmp(toc->keyword, cmds[0]) == 0) {
            this = toc;
            FREE_UNTIL_NULL(cmds);
            return 1;
        }
    FREE_UNTIL_NULL(cmds);
    return 0;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
void do_builtin(const char **argv)
{
    char **real_cmd = getArgs(argv[0]);
    this->do_it((const char **) real_cmd);
    FREE_UNTIL_NULL(real_cmd);
}
