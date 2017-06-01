/*
*session.c
*
*  Created on: 2016-11-8
*      Author: lc
*/

#include <glib.h>

#include "session.h"
#include "log.h"
#include "object.h"

static GHashTable *session_table = NULL;

void session_freeValue(gpointer value)
{
    SESSION *session = (SESSION *)value;

    LOG_DEBUG("free value IMEI:%s of session_table", ((OBJECT *)session->obj)->IMEI);

    g_free(session);
}

void session_table_initial(void)
{
    session_table = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, session_freeValue);
}

void session_table_destruct(void)
{
    g_hash_table_destroy(session_table);
}

int session_add(SESSION *session)
{
    OBJECT *t_obj = (OBJECT *)(session->obj);

    g_hash_table_insert(session_table, session->bev, session);
    LOG_INFO("session(%s) added", t_obj->IMEI);
    return 0;
}

int session_del(SESSION *session)
{
    if(!session)
    {
        LOG_ERROR("session not exist");
        return -1;
    }

    OBJECT *t_obj = (OBJECT *)(session->obj);
    if (!t_obj)
    {
        LOG_WARN("object not login before timeout");
        return 0;
    }

    if(session == t_obj->session)
    {
        t_obj->session = NULL;
        LOG_WARN("device (%s)'s session deleted", t_obj->IMEI);
    }
    g_hash_table_remove(session_table, (gconstpointer)(session->bev));
    LOG_INFO("session(%s) deleted", t_obj->IMEI);
    return 0;
}

