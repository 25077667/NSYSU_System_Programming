/*
 * is_background.c :  check for & at end
 */

#include <string.h>
#include "shell.h"

int is_background(char **myArgv)
{
    if (*myArgv == NULL)
        return FALSE;

    /* Look for "&" in myArgv, and process it.
     *
     *	- Return TRUE if found.
     *	- Return FALSE if not found.
     *
     * Fill in code.
     */

    int i = -1;
    while (myArgv[++i] && strcmp(myArgv[i], "&") != 0)
        ;
    return !!(myArgv[i]);
}