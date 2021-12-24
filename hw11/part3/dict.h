/*
 * header file for progressive dictionary exercise
 */

#include <stdio.h>
#include <stdlib.h>

#define WORD 32
#define TEXT 480

#define FOUND 1
#define NOTFOUND 0
#define UNAVAIL -1

#define DIE(x) perror(x), exit(1)
#define EE(expr)                       \
    do {                               \
        __auto_type val = (expr);      \
        if (val == (typeof(val)) (-1)) \
            DIE(#expr);                \
    } while (0)

#define Wunused(expr)   \
    do {                \
        if (!!(expr)) { \
        }               \
    } while (0)

/* standard lookup structure */
typedef struct {
    char word[WORD];
    char text[TEXT];
} Dictrec;

int my_connect(const char *name);
int lookup(Dictrec *sought, const char *name);
int my_disconnect(void);