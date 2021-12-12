/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* svmsg_file_server.c

   A file server that uses System V message queues to handle client requests
   (see svmsg_file_client.c). The client sends an initial request containing
   the name of the desired file, and the identifier of the message queue to be
   used to send the file contents back to the child. The server attempts to
   open the desired file. If the file cannot be opened, a failure response is
   sent to the client, otherwise the contents of the requested file are sent
   in a series of messages.

   This application makes use of multiple message queues. The server maintains
   a queue (with a well-known key) dedicated to incoming client requests. Each
   client creates its own private queue, which is used to pass response
   messages from the server back to the client.

   This program operates as a concurrent server, forking a new child process to
   handle each client request while the parent waits for further client
   requests.
*/
#include "pmsg_file.h"

static mqd_t serverId;

static void __clean_serverId(int sig)
{
    if (sig == SIGCHLD) {
        int savedErrno = errno; /* waitpid() might change 'errno' */
        while (waitpid(-1, NULL, WNOHANG) > 0)
            continue;
        errno = savedErrno;
    } else {
        puts("clean server id");
        mq_close(serverId);
        mq_unlink(SERVER_KEY);
        exit(0);
    }
}

/* Executed in child process: serve a single client */
static void serveRequest(const struct requestMsg *req)
{
    const int clientId = mq_open(req->clientPath, O_RDWR);
    const int fd = open(req->pathname, O_RDONLY);
    if (fd == -1) { /* Open failed: send error text */
        struct responseMsg resp = {.mtype = MT_FAILURE,
                                   .data = "Couldn't open "};
        strcat(resp.data, req->pathname);
        mq_send(clientId, (const char *) &resp, sizeof(struct responseMsg), 0);
        mq_close(clientId);
        exit(EXIT_FAILURE);
    }

    /* Transmit file contents in messages with type RESP_MT_DATA. We don't
       diagnose read() and mq_send() errors since we can't notify client. */
    struct responseMsg resp = {.mtype = MT_DATA, .data = {0}};
    while ((read(fd, resp.data, MSG_SIZE)) > 0)
        if (mq_send(clientId, (const char *) &resp, sizeof(struct responseMsg),
                    0) == -1) {
            perror("mq_send");
            break;
        }

    /* Send a message of type RESP_MT_END to signify end-of-file */
    resp.mtype = MT_END;
    mq_send(clientId, (const char *) &resp, sizeof(struct responseMsg),
            0); /* Zero-length mtext */
    mq_close(clientId);
}

int main(void)
{
    struct sigaction sa = {.sa_flags = 0, .sa_handler = __clean_serverId};
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGCHLD, &sa, NULL);

    /* Create server message queue */
    serverId =
        mq_open(SERVER_KEY, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR, NULL);
    if (serverId == -1 && errno == EEXIST)
        serverId = mq_open(SERVER_KEY, O_RDWR, S_IRUSR | S_IWUSR, NULL);
    else if (serverId == -1)
        errExit("mq_open");

    /* Read requests, handle each in a separate child process */
    while (1) {
        struct requestMsg req;
        ssize_t msgLen =
            mq_receive(serverId, (char *) &req, sizeof(struct requestMsg), 0);
        if (msgLen == -1) {
            if (errno == EINTR)   /* Interrupted by SIGCHLD handler? */
                continue;         /* ... then restart mq_receive() */
            errMsg("mq_receive"); /* Some other error */
            break;                /* ... so terminate loop */
        }

        if (!fork()) { /* Child */
#ifdef DEBUG
            fputs("Got req!", stderr);
#endif
            serveRequest(&req);
            return 0;
        }
        /* Parent loops to receive next client request */
    }

    mq_close(serverId);
    mq_unlink(SERVER_KEY);
    return 0;
}
