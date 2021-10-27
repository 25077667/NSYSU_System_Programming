/*
 *  pipe_present.c :  check for |
 */

#include <stdio.h>
#include <string.h>
#include "shell.h"

/*
 * Return index offset into argv of where "|" is,
 * -1 if in an illegal position (first or last index in the array),
 * or 0 if not present.
 */
int pipe_present(char **myCurrentArgv)
{
    int index = 0;

    /* Search through myCurrentArgv for a match on "|". */
    while (myCurrentArgv[index] && strcmp(myCurrentArgv[index++], "|"))
        ;

    if (index == 1 || !strcmp(myCurrentArgv[index - 1],
                              "|")) /* At the beginning or at the end. */
        return -1;
    else if (myCurrentArgv[index] == NULL) /* Off the end. */
        return 0;
    else /* In the middle. */
        return index - 1;
}
