/*
*session.h
*
*  Created on: 2016-11-8
*      Author: lc
*/
#ifndef ELECTROMBILE_SESSION_H
#define ELECTROMBILE_SESSION_H

#include <glib.h>
#include <stddef.h>
#include <event2/bufferevent.h>

typedef void (*MSG_SEND)(struct bufferevent* bev, const void* buf, size_t n);

typedef struct
{
    struct event_base *base;
    struct bufferevent *bev;

    void *obj;
    MSG_SEND pSendMsg;
}SESSION;

void session_table_initial(void);
void session_table_destruct(void);

int session_add(SESSION *);
int session_del(SESSION *);

#endif //ELECTROMBILE_SESSION_H
