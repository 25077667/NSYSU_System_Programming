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
#include "svmsg_file.h"

static int clientId;

static void __removeQueue(void)
{
    if (msgctl(clientId, IPC_RMID, NULL) == -1)
        errExit("msgctl");
}

int main(int argc, char *argv[])
{
    // Error usage
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    if (strlen(argv[1]) > 4095)
        cmdLineErr("pathname too long (max: %d bytes)\n", 4095);

    /* Get server's queue identifier; create queue for response */
    int serverId = msgget(SERVER_KEY, S_IWUSR);
    if (serverId == -1)
        errExit("msgget - server message queue");

    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId == -1)
        errExit("msgget - client message queue");

    if (atexit(__removeQueue) != 0)
        errExit("atexit");

    /* Send message asking for file named in argv[1] */
    struct requestMsg req = {.clientId = clientId, .pathname = {0}};
    strcpy(req.pathname, argv[1]);
    size_t reqlen = strlen(req.pathname) + 1;
    if (msgsnd(serverId, &req, reqlen, 0) == MT_FAILURE)
        errExit("msgsnd");

    // Process response from server
    ssize_t totBytes = 0, numMsgs = -1;
    struct responseMsg resp = {.mtype = MT_DATA, .data = {0}};
    for (; resp.mtype; ++numMsgs) {
        ssize_t msgLen = msgrcv(clientId, &resp, MSG_SIZE, 0, 0);
        if (__glibc_unlikely(resp.mtype == MT_FAILURE)) {
            printf("%s\n", resp.data); /* Display msg from server */
            exit(EXIT_FAILURE);
        }
        totBytes += msgLen * resp.mtype;  // END is 0
    }

    printf("Received %ld bytes (%ld messages)\n", totBytes, numMsgs);
    exit(EXIT_SUCCESS);
}
