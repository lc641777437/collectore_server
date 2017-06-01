/*
 * object.c
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>

#include "log.h"
#include "object.h"

static GHashTable *object_table = NULL;

static void obj_add_hash(OBJECT *obj)
{
	const char* strIMEI = obj->IMEI;
	g_hash_table_insert(object_table, g_strdup(strIMEI), obj);
    LOG_INFO("obj %s added to hashtable", strIMEI);
}

void obj_freeKey(gpointer key)
{
    LOG_DEBUG("free key IMEI:%s of object_table", (char *)key);
    g_free(key);
}

void obj_freeValue(gpointer value)
{
    OBJECT *obj = (OBJECT *)value;

    LOG_DEBUG("free value IMEI:%s of object_table", get_IMEI_STRING(obj->IMEI));

    g_free(obj);
}

void obj_table_initial(void)
{
    object_table = g_hash_table_new_full(g_str_hash, g_str_equal, obj_freeKey, obj_freeValue);
}

void obj_table_destruct(void)
{
    g_hash_table_destroy(object_table);
}

static void make_pwd(char pwd[])
{
    srand(time(NULL));

    for(int i = 0; i < MAX_PWD_LEN; i++)
	{
        pwd[i] = 65 + rand() % (90 - 65);
	}
    pwd[MAX_PWD_LEN - 1] = '\0';

    return;
}

OBJECT *obj_new(void)
{
	OBJECT * obj = g_malloc(sizeof(OBJECT));
	memset(obj, 0, sizeof(OBJECT));

	return obj;
}

void obj_add(OBJECT *obj)
{
	obj_add_hash(obj);
}

void obj_del(OBJECT *obj)
{
    OBJECT * t_obj = obj_get(obj->IMEI);
    if(NULL != t_obj)
    {
        g_hash_table_remove(object_table, obj->IMEI);
    }
}

OBJECT *obj_get(const char IMEI[])
{
    return g_hash_table_lookup(object_table, IMEI);
}

const char *get_IMEI_STRING(const char *IMEI)
{
	static char strIMEI[IMEI_LENGTH + 2];
	strcpy(strIMEI, "unknown imei");

	if (!IMEI)
	{
		return strIMEI;
	}
	for (int i = 0; i < ((IMEI_LENGTH + 1) / 2); i++)
	{
		sprintf(strIMEI + i * 2, "%02x", IMEI[i]&0xff);
	}
	strIMEI[IMEI_LENGTH + 1] = 0;

	return strIMEI + 1;
}

