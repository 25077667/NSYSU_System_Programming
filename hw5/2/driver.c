#include <stdarg.h>
#include <string.h>
#include "popen.h"
// #include <stdio.h>

#define PAGE_SIZE 4096

extern void format_cmd(char *cmd, const char *tok);
#define FORMAT_CMD(cmd, tok) (format_cmd)(cmd, tok)

#define __DECLARE_TESTS_r(cmd, mode)                    \
    static inline void read_##cmd(const char *arg, ...) \
    {                                                   \
        char __cmd[PAGE_SIZE] = {0};                    \
        strcat(__cmd, #cmd);                            \
        static char buf[PAGE_SIZE] = {0};               \
        va_list args;                                   \
        va_start(args, arg);                            \
        char *tok = (char *) arg;                       \
        do {                                            \
            FORMAT_CMD(__cmd, tok);                     \
        } while ((tok = va_arg(args, char *)) != NULL); \
        /* ---------------------------------------- */; \
        FILE *f = popen(__cmd, #mode);                  \
        while (fread(buf, 1, PAGE_SIZE, f))             \
            fwrite(buf, 1, PAGE_SIZE, stdout);          \
        pclose(f);                                      \
    }

#define __DECLARE_TESTS_w(cmd, mode)                             \
    static inline void write_##cmd(const char *buf, ssize_t len, \
                                   const char *filename, ...)    \
    {                                                            \
        char __cmd[PAGE_SIZE] = {0};                             \
        strcat(__cmd, #cmd);                                     \
        va_list args;                                            \
        va_start(args, filename);                                \
        char *tok = (char *) filename;                           \
        do {                                                     \
            FORMAT_CMD(__cmd, tok);                              \
        } while ((tok = va_arg(args, char *)) != NULL);          \
        /* ---------------------------------------- */;          \
        FILE *f = popen(__cmd, #mode);                           \
        fwrite(buf, 1, len, f);                                  \
        pclose(f);                                               \
    }

#define DECLARE_TESTS(cmd, mode) __DECLARE_TESTS_##mode(cmd, mode)

DECLARE_TESTS(tee, w);
DECLARE_TESTS(ls, r);
DECLARE_TESTS(chmod, r);
DECLARE_TESTS(bash, r);
DECLARE_TESTS(rm, r);
DECLARE_TESTS(date, r);

int main(void)
{
    const char *filename = "hello.sh";
    const char ctx[] =
        "#!/bin/bash\n"
        "echo hello\n";

    printf("tee ```%s``` to file %s\n", ctx, filename);
    write_tee(ctx, sizeof(ctx), filename, NULL);

    printf("chmod %s %s\n", "700", filename);
    read_chmod("700", filename, NULL);

    printf("ls:\n");
    read_ls("-l", NULL);

    printf("Exec %s:\n", filename);
    read_bash(filename, NULL);

    printf("rm %s\n", filename);
    read_rm(filename, NULL);

    read_date(NULL);
    return 0;
}