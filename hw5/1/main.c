#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define Wunused(expr)   \
    do {                \
        if (!!(expr)) { \
        };              \
    } while (0)

void parent(int out, int in)
{
    size_t BUF_SIZE = getpagesize();
    char buf1[BUF_SIZE], buf2[BUF_SIZE];

    ssize_t len = 0;
    while (!!(len = read(STDIN_FILENO, buf1, BUF_SIZE))) {
        if (__glibc_unlikely(write(out, buf1, len) == -1)) {
            perror("P to C write");
            break;
        }
        ssize_t len_from_child = read(in, buf2, BUF_SIZE);
        if (__glibc_unlikely(len_from_child == -1)) {
            perror("C to P read");
            break;
        }
        Wunused(write(STDOUT_FILENO, buf2, len_from_child));
    }
    if (__glibc_unlikely(len == -1))
        perror("stdin");
}

void child(int out, int in)
{
    size_t BUF_SIZE = getpagesize();
    char buf1[BUF_SIZE], buf2[BUF_SIZE];

    ssize_t len = 0;
    while (!!(len = read(in, buf1, BUF_SIZE))) {
        for (int i = 0; i < len; i++)
            buf2[i] = toupper(buf1[i]);
        if (__glibc_unlikely(write(out, buf2, len) == -1)) {
            perror("C to P write");
            break;
        }
    }
    if (__glibc_unlikely(len == -1))
        perror("P to C read");
}

int main()
{
    // Preparing the bidirectional pipes
    int p2c[2], c2p[2];
    if (pipe(p2c) != 0 || pipe(c2p) != 0)
        perror("pipe");

    if (fork()) {
        close(p2c[0]);  // Disable parent to child input fd
        close(c2p[1]);
        parent(p2c[1], c2p[0]);
    } else {
        close(p2c[1]);
        close(c2p[0]);
        child(c2p[1], p2c[0]);
    }

    return 0;
}