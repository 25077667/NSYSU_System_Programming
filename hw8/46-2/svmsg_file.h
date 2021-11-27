/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* svmsg_file.h

   Header file for svmsg_file_server.c and svmsg_file_client.c.
*/
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h> /* For definition of offsetof() */
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

#define SERVER_KEY 0xdeadbeaf /* Key for server's message queue */

enum { MSG_SIZE = 8192 };
struct requestMsg {          /* Requests (client to server) */
    int clientId;            /* ID of client's message queue */
    char pathname[MSG_SIZE]; /* File to be returned */
};
#define REQ_MSG_SIZE                         \
    (offsetof(struct requestMsg, pathname) - \
     offsetof(struct requestMsg, clientId) + PATH_MAX)

// RESPONSE
struct responseMsg {     /* Responses (server to client) */
    int mtype;           /* One of RESP_MT_* values below */
    char data[MSG_SIZE]; /* File content / response message */
};

/* Types for response messages sent from server to client */
#define MT_FAILURE -1 /* File couldn't be opened */
#define MT_DATA 1     /* Message contains file data */
#define MT_END 0      /* File data complete */