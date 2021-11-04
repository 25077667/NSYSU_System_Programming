#include "popen.h"
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include "dict.hpp"

extern void insert(int pid, FILE *fp);
extern void drop();
extern void erase(FILE *fp);
extern int get_pid(FILE *fp);

// The type must be "w" or "r"
FILE *my_popen(const char *command, const char *type)
{
    const int is_write = !!(*type ^ 'r');
    int fd[2];
    if (pipe(fd))
        perror("pipe");

    pid_t pid = fork();
    if (!pid) {  // Child
        drop();  // Drop the data structure
        close(fd[is_write]);

        if (fd[!is_write] != !is_write) {
            dup2(fd[!is_write], !is_write);
            close(fd[!is_write]);
        }

        execl("/bin/sh", "sh", "-c", command, NULL);
        _exit(127);
    }

    FILE *ret_f = fdopen(fd[is_write], type);
    close(fd[!is_write]);

    insert(pid, ret_f);  // Add to data structure
    return ret_f;
}

int my_pclose(FILE *stream)
{
    int pstat = 0;
    pid_t pid = get_pid(stream);  // Get the data structure

    fclose(stream);
    erase(stream);  // Erase the data structure

    do {
        pid = waitpid(pid, &pstat, 0);
    } while (pid == -1 && errno == EINTR);

    return pstat;
}