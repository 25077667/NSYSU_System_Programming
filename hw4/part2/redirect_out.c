/*
 * redirect_out.c   :   check for >
 */

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

/*
 * Look for ">" in myArgv, then redirect output to the file.
 * Returns 0 on success, sets errno and returns -1 on error.
 */
int redirect_out(char **myArgv)
{
    int i = 0;

    /* search forward for >
     * Fill in code. */
    char *tok = NULL;
    do {
        tok = myArgv[i++];
        if (tok == NULL)
            return -1;
    } while (strcmp(tok, ">"));

    if (myArgv[i]) { /* found ">" in vector. */
                     // myArgv[i] is the filename

        int fd = open(myArgv[i], O_WRONLY);
        /* 1) Open file.
         * 2) Redirect to use it for output.
         * 3) Cleanup / close unneeded file descriptors.
         * 4) Remove the ">" and the filename from myArgv.
         *
         * Fill in code. */
        dup2(fd, STDOUT_FILENO);
        int j = i;
        while (myArgv[++j] != NULL)
            ;

        free(myArgv[i - 1]);
        free(myArgv[i]);
        memcpy(&myArgv[i - 1], &myArgv[i + 1], j - i);
    }
    return 0;
}
