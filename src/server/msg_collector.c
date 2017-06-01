/*
 * msg_simcom.c
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */
#include <string.h>
#include <malloc.h>
#include <netinet/in.h>

#include "protocol.h"
#include "msg_collector.h"

static char seq = 0;

MSG_HEADER* alloc_simcomMsg(char cmd, size_t length)
{
    MSG_HEADER* msg = malloc(length);

    if(msg)
    {
        msg->signature = htons(START_FLAG);
        msg->cmd = cmd;
        msg->seq = seq++;
        msg->length = htons(length - MSG_HEADER_LEN);
    }

    return msg;
}

void free_simcom_msg(void *msg)
{
    free(msg);
}

char get_msg_cmd(void *msg)
{
    return ((MSG_HEADER*)msg)->cmd;
}

