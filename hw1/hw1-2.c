// An implementation of the `cp` command
// Author: SCC

#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (__glibc_unlikely(argc != 3)) {
        fprintf(stderr, "DO NOT SUPPORT THIS OPERATION!\n");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w+");
    int c;
    while ((c = fgetc(in)) != EOF)
        fputc(c, out);

    fclose(in);
    fclose(out);
    return 0;
}