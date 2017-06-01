/*
 * mc_object.h
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */

#ifndef SRC_OBJECT_
#define SRC_OBJECT_

#include "macro.h"
#include "session.h"


typedef struct
{
//    char DID[MAX_DID_LEN];
//    char clientID[CLIENT_ID_LEN];
    short cmd;
    unsigned short seq;
}APP_SESSION;


typedef struct
{
    char IMEI[IMEI_LENGTH + 1];

    int timestamp;
    float latitute;
    float longitute;

    void *session;
} OBJECT;

void obj_table_initial(void);
void obj_table_destruct(void);

OBJECT *obj_new(void);
void obj_add(OBJECT *obj);
void obj_del(OBJECT *obj);
OBJECT *obj_get(const char IMEI[]);

int obj_did_got(OBJECT *obj);
const char* get_IMEI_STRING(const char* IMEI);

#endif /* SRC_OBJECT_ */
