/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* svmsg_file_client.c

   Send a message to the server svmsg_file_server.c requesting the
   contents of the file named on the command line, and then receive the
   file contents via a series of messages sent back by the server. Display
   the total number of bytes and messages received. The server and client
   communicate using System V message queues.
*/
#include "pmsg_file.h"

#define FOO_PATH "/foo_"

mqd_t clientId;

static void __clean_clientId(int sig)
{
    if (sig == SIGCHLD) {
        int savedErrno = errno; /* waitpid() might change 'errno' */
        while (waitpid(-1, NULL, WNOHANG) > 0)
            continue;
        errno = savedErrno;
    } else {
        puts("clean client id");
        mq_close(clientId);
        mq_unlink(FOO_PATH);
        exit(0);
    }
}

static void connect_server(mqd_t serverId, const char *name)
{
    struct requestMsg req = {.clientPath = FOO_PATH, .pathname = {0}};
    strcpy(req.pathname, name);
    if (mq_send(serverId, (const char *) &req, sizeof(struct requestMsg), 0) ==
        MT_FAILURE)
        errExit("mq_send");
    mq_close(serverId);
}

static void proc_resp()
{
    struct responseMsg resp = {.mtype = MT_DATA, .data = {0}};
    while (resp.mtype) {
        if (write(STDOUT_FILENO, resp.data, MSG_SIZE)) {
        }
        if (mq_receive(clientId, (char *) &resp, sizeof(struct responseMsg),
                       0) == -1)
            perror("mq_receive");
        if (__glibc_unlikely(resp.mtype == MT_FAILURE)) {
            printf("%s\n", resp.data); /* Display msg from server */
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[])
{
    struct sigaction sa = {.sa_flags = 0, .sa_handler = __clean_clientId};
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);

    // Error usage
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    if (strlen(argv[1]) > 4095)
        cmdLineErr("pathname too long (max: %d bytes)\n", 4095);

    /* Get server's queue identifier; create queue for response */
    mqd_t serverId = mq_open(SERVER_KEY, O_RDWR);
    if (serverId == -1)
        errExit("mq_open - server message queue");

    clientId =
        mq_open(FOO_PATH, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR, NULL);
    if (clientId == -1 && errno == EEXIST)
        clientId = mq_open(FOO_PATH, O_RDWR, S_IRUSR | S_IWUSR, NULL);
    else if (clientId == -1)
        errExit("mq_open");

    connect_server(serverId, argv[1]);
    proc_resp();

    mq_close(clientId);
    mq_unlink(FOO_PATH);
    exit(EXIT_SUCCESS);
}
