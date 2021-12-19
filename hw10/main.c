#include <fcntl.h> /* For O_* constants */
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "Broadcast.h"
#include "Consumer.h"
#include "Producer.h"

struct Tuple {
    unsigned int data_num;
    useconds_t sleep_time;
    unsigned int consumer_num;
    unsigned int buffer_num;
};

#define Wunused(expr)   \
    do {                \
        if (!!(expr)) { \
        };              \
    } while (0)

// static const int AVA_SIG[] = {SIGALRM, SIGUSR1, SIGUSR2, SIGBUS,
//                         SIGTSTP, SIGXCPU, SIGPOLL, SIGXFSZ};
static const int AVA_SIG[] = {SIGUSR1, SIGUSR2};

static inline unsigned int collect_child(int n, int r, int m)
{
    unsigned int failed_sum = 0;
    siginfo_t info = {._sifields._rt.si_sigval.sival_int =
                          0};  // For optimization
    sigset_t set;
    sigemptyset(&set);
    for (unsigned long i = 0; i < (sizeof(AVA_SIG) / sizeof(int)); i++)
        sigaddset(&set, AVA_SIG[i]);
    sigprocmask(SIG_BLOCK, &set, NULL);

    const struct timespec timeout = {.tv_sec = log10(n) * m * r / 1000};
    for (int i = 0; i < n; i++) {
        while (sigtimedwait(&set, &info, &timeout) == -1) {
            perror("sigwait");
            goto end;
        }
        failed_sum += info.si_value.sival_int;

#ifdef DEBUG_GOT
        fprintf(stderr, "tot: %d, got %d from %d\n", failed_sum,
                info.si_value.sival_int, i);
#endif
    }
end:
    return failed_sum;
}

int main(int argc, char *argv[])
{
    if (argc != 5) {  // On err
        static const char *help_str =
            "Usage: %s [M] [R] [N] [B]\n"
            "        M: number of messages\n"
            "        R: transmission interval in millisecond\n"
            "        N: number of consumers\n"
            "        B: number of slots of transmission buffer\n";
        fprintf(stderr, help_str, argv[0]);
        exit(-1);
    }

    const struct Tuple t = {.data_num = atoi(argv[1]),
                            .sleep_time = atoi(argv[2]),
                            .consumer_num = atoi(argv[3]),
                            .buffer_num = atoi(argv[4])};
    char SHM_FILE[128] = {0};
    snprintf(SHM_FILE, 128, "/DEADBEAF%d_%d_%d_%d", t.data_num, t.sleep_time,
             t.consumer_num, t.buffer_num);
    const int shm_fd = shm_open(SHM_FILE, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG);
    const unsigned int REAL_BROADCAST_SIZE =
        (t.buffer_num + 1) * sizeof(struct Broadcast);

    Wunused(ftruncate(shm_fd, REAL_BROADCAST_SIZE));
    struct Broadcast *b_array = (struct Broadcast *) mmap(
        NULL, REAL_BROADCAST_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,
        0);
    memset(b_array, 0, REAL_BROADCAST_SIZE);

    int n = t.consumer_num;
    while (n--) {       // Childs are consumers
        if (!fork()) {  // Child
            struct Consumer c = {.work_id = n,
                                 .data_num = t.data_num,
                                 .buffer_num = t.buffer_num,
                                 .buf = b_array,
                                 .job = consumer_job};
#ifdef DEBUG_CONSUMER
            fprintf(stderr, "gen c %d\n", n);
#endif
            const int fail_num = c.job(&c);
            const union sigval ret = {.sival_int = fail_num};

            const int sig_num = AVA_SIG[n % (sizeof(AVA_SIG) / sizeof(int))];
            if (sigqueue(getppid(), sig_num, ret))
                perror("sigqueue");
            exit(0);
        }
    }

    if (!fork()) {  // Create producer process
        struct Producer p = {.message_num = t.data_num,
                             .sleep_time = t.sleep_time,
                             .buffer_num = t.buffer_num,
                             .buf = b_array,
                             .job = producer_job};
        p.job(&p);
        exit(0);
    }

    const unsigned int failed_sum =
        collect_child(t.consumer_num, t.sleep_time, t.data_num);
    const unsigned int tot_msg = t.data_num * t.consumer_num;

    shm_unlink(SHM_FILE);
    munmap(b_array, REAL_BROADCAST_SIZE);
#ifdef GEN_DATA
    printf("%.5f\n", 1.0 * failed_sum / tot_msg);
#else
    printf("M=%d  R=%dms  N=%d\n", t.data_num, t.sleep_time, t.consumer_num);
    puts("-------------------------");
    printf("Total message: %d\n", tot_msg);
    printf("Sum of received messages by all consumers: %d\n",
           tot_msg - failed_sum);
    printf("Loss rate: %.5f%%\n", 1.0 * failed_sum / tot_msg);
    puts("-------------------------");
#endif
    return 0;
}