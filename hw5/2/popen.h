#ifndef __POPEN_H__
#define __POPEN_H__

#include <stdio.h>

FILE *my_popen(const char *command, const char *type);
int my_pclose(FILE *stream);

// Hooks
#define popen my_popen
#define pclose my_pclose

#endif