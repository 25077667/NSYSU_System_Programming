/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* svshm_xfr_reader.c

   This is a reader daemon, which read the data provided by writer.
    see svshm_xfr_writer.c
*/
#include "svshm_xfr.h"

int semid;
struct shmseg *shmp;

static void __clean_shmid(int sig)
{
    (void) sig;
    puts("clean reader shm");
    EE(releaseSem(semid, WRITE_SEM));
    EE(munmap(shmp, sizeof(struct shmseg)));
    exit(0);
}

int main(void)
{
    struct sigaction sa = {.sa_flags = 0, .sa_handler = __clean_shmid};
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);

    /* Get IDs for semaphore set and shared memory created by writer */
    EE(semid = semget(SEM_KEY, 0, 0));

    int shmid = 0;
    EE(shmid = shm_open(SHM_KEY, O_RDWR, OBJ_PERMS));


    /* Attach shared memory read-only, as we will only read */
    EE(shmp = (struct shmseg *) mmap(NULL, sizeof(struct shmseg), PROT_READ,
                                     MAP_SHARED, shmid, 0));

    /* Transfer blocks of data from shared memory to stdout */
    int xfrs = 0, bytes = 0;
    for (;; xfrs++) {
        EE(reserveSem(semid, READ_SEM)); /* Wait for our turn */

        if (shmp->cnt == 0) /* Writer encountered EOF */
            break;
        bytes += shmp->cnt;

        if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
            fatal("partial/failed write");

        EE(releaseSem(semid, WRITE_SEM)); /* Give writer a turn */
    }
    /* Give writer one more turn, so it can clean up */
    EE(releaseSem(semid, WRITE_SEM));

    fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
    EE(munmap(shmp, sizeof(struct shmseg)));
    return 0;
}
