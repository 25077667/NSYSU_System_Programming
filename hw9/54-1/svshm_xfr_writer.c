/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/*  svshm_xfr_writer.c

   Read buffers of data data from standard input into a System V shared memory
   segment from which it is copied by svshm_xfr_reader.c

   We use a pair of binary semaphores to ensure that the writer and reader have
   exclusive, alternating access to the shared memory. (I.e., the writer writes
   a block of text, then the reader reads, then the writer writes etc). This
   ensures that each block of data is processed in turn by the writer and
   reader.

   This program needs to be started before the reader process as it creates the
   shared memory and semaphores used by both processes.

   Together, these two programs can be used to transfer a stream of data through
   shared memory as follows:

        $ svshm_xfr_writer < infile &
        $ svshm_xfr_reader > out_file
*/
#include "semun.h" /* Definition of semun union */
#include "svshm_xfr.h"

int semid;
struct shmseg *shmp;

static void __clean_shmid(int sig)
{
    (void) sig;
    puts("clean writer shm");
    EE(releaseSem(semid, WRITE_SEM));
    shm_unlink(SHM_KEY);
    EE(munmap(shmp, sizeof(struct shmseg)));
    exit(0);
}


int main(void)
{
    struct sigaction sa = {.sa_flags = 0, .sa_handler = __clean_shmid};
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);

    /* Create set containing two semaphores; initialize so that
       writer has first access to shared memory. */
    semid = 0;
    EE(semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS));
    EE(initSemAvailable(semid, WRITE_SEM));
    EE(initSemInUse(semid, READ_SEM));

    /* Create shared memory */
    int shmid = 0;
    EE(shmid = shm_open(SHM_KEY, O_CREAT | O_RDWR, OBJ_PERMS));

    EE(ftruncate(shmid, sizeof(struct shmseg)));
    EE(shmp = (struct shmseg *) mmap(NULL, sizeof(struct shmseg),
                                     PROT_READ | PROT_WRITE, MAP_SHARED, shmid,
                                     0));

    /* Transfer blocks of data from stdin to shared memory */
    int bytes = 0, xfrs = 0;
    do {
        EE(reserveSem(semid, WRITE_SEM)); /* Wait for our turn */
        EE(shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE));
        EE(releaseSem(semid, READ_SEM)); /* Give reader a turn */
    } while (!!(shmp->cnt) && (++xfrs, bytes += shmp->cnt));

    /* Wait until reader has let us have one more turn. We then know
       reader has finished, and so we can delete the IPC objects. */
    union semun dummy;
    EE(reserveSem(semid, WRITE_SEM));
    EE(semctl(semid, 0, IPC_RMID, dummy));
    EE(munmap(shmp, sizeof(struct shmseg)));
    EE(shm_unlink(SHM_KEY));

    fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
    return 0;
}
