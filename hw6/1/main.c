#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>
#include "lib.h"

#define __RESET_NAME(name) \
    do {                   \
        if (name)          \
            free(name);    \
        name = NULL;       \
    } while (0)

static int inotify_fd;

#ifdef __DEBUG__
static void display_inotify_event(const struct inotify_event *const i)
{
    fputs("mask = ", stderr);
    if (i->mask & IN_ACCESS)
        fputs("IN_ACCESS ", stderr);
    if (i->mask & IN_ATTRIB)
        fputs("IN_ATTRIB ", stderr);
    if (i->mask & IN_CLOSE_NOWRITE)
        fputs("IN_CLOSE_NOWRITE ", stderr);
    if (i->mask & IN_CLOSE_WRITE)
        fputs("IN_CLOSE_WRITE ", stderr);
    if (i->mask & IN_CREATE)
        fputs("IN_CREATE ", stderr);
    if (i->mask & IN_DELETE)
        fputs("IN_DELETE ", stderr);
    if (i->mask & IN_DELETE_SELF)
        fputs("IN_DELETE_SELF ", stderr);
    if (i->mask & IN_IGNORED) {
        erase_watch_file(i->wd);
        fputs("IN_IGNORED ", stderr);
    }
    if (i->mask & IN_ISDIR)
        fputs("IN_ISDIR ", stderr);
    if (i->mask & IN_MODIFY)
        fputs("IN_MODIFY ", stderr);
    if (i->mask & IN_MOVE_SELF)
        fputs("IN_MOVE_SELF ", stderr);
    if (i->mask & IN_MOVED_FROM)
        fputs("IN_MOVED_FROM ", stderr);
    if (i->mask & IN_MOVED_TO)
        fputs("IN_MOVED_TO ", stderr);
    if (i->mask & IN_OPEN)
        fputs("IN_OPEN ", stderr);
    if (i->mask & IN_Q_OVERFLOW)
        fputs("IN_Q_OVERFLOW ", stderr);
    if (i->mask & IN_UNMOUNT) {
        erase_watch_file(i->wd);
        fputs("IN_UNMOUNT ", stderr);
    }
    fprintf(stderr, " name = %s\n", i->name);
}
#endif

void add_notify(const char *path, int base)
{
    int wd = inotify_add_watch(inotify_fd, path, IN_ALL_EVENTS);
    add_watch_file(path, base, wd);
}

static int __add_watch_file(const char *fpath,
                            const struct stat *sb,
                            int tflag,
                            struct FTW *ftwbuf)
{
    (void) sb;
    (void) tflag;
    add_notify(fpath, ftwbuf->base);
    return 0; /* To tell nftw() to continue */
}

int main(int argc, char **argv)
{
    enum { PAGE_SIZE = 4096 };
    int flags = FTW_PHYS;
    static char buf[PAGE_SIZE] = {0};
    const char *path = (argc < 2) ? "." : argv[1];

    if (argc > 2 && strchr(argv[2], 'd') != NULL)
        flags |= FTW_DEPTH;
    if (argc > 2 && strchr(argv[2], 'p') != NULL)
        flags &= ~FTW_PHYS;

    inotify_fd = inotify_init();

    nftw(path, __add_watch_file, 64, flags);

    while (1) {
        const int num = read(inotify_fd, buf, PAGE_SIZE);
        char *fname = NULL;
        char *tname = NULL;

        for (char *p = buf; p < buf + num;) {
            const struct inotify_event *const e = (struct inotify_event *) p;
            const char *dir = wd2dir(e->wd);
            if (e->mask & IN_CREATE) {
                const char *const new_name = e->name;
                add_notify(concat(dir, new_name), strlen(dir));
                printf("A new file '%s' was created under '%s'\n", new_name,
                       dir);
            }
            if (e->mask & IN_DELETE) {
                printf("A file '%s' was deleted.\n", e->name);  // old name
            }

            if (e->mask & IN_MOVED_TO) {  // Move in
                tname = strdup(concat(dir, e->name));
                add_notify(tname, strlen(dir));
                printf("Move in to %s\n", tname);
            }
            if (e->mask & IN_MOVED_FROM) {  // Move out
                fname = strdup(concat(dir, e->name));
                printf("Move out from %s\n", fname);
            }
            if (e->mask & IN_MOVE_SELF) {
                printf("The %s was renamed as %s\n", fname, tname);
                __RESET_NAME(fname);
                __RESET_NAME(tname);
            }

#ifdef __DEBUG__
            display_inotify_event(e);
#endif
            p += sizeof(struct inotify_event) + e->len;
        }
    }

    return 0;
}