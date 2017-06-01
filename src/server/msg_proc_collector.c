/*
 * msg_proc_simcom.c
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */


#include <string.h>
#include <netinet/in.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <object.h>

#include "log.h"
#include "object.h"
#include "protocol.h"
#include "msg_collector.h"
#include "msg_proc_collector.h"

typedef int (*MSG_PROC)(const void *msg, SESSION *ctx);
typedef struct
{
    char cmd;
    MSG_PROC pfn;
} MSG_PROC_MAP;

static int simcom_sendMsg(void *msg, size_t len, SESSION *session)
{
    if (!session)
    {
        return -1;
    }

    MSG_SEND pfn = session->pSendMsg;
    if (!pfn)
    {
        LOG_ERROR("device offline");
        return -1;
    }
    pfn(session->bev, msg, len);

    LOG_DEBUG("send msg(cmd=%d), length(%ld)", get_msg_cmd(msg), len);
    LOG_HEX(msg, len);
    free_simcom_msg(msg);

    return 0;
}

static time_t get_time()
{
    time_t rawtime;
    time(&rawtime);
    return rawtime;
};

static int simcom_login(const void *msg, SESSION *session)
{
    const MSG_LOGIN_REQ *req = (const MSG_LOGIN_REQ *)msg;
    if(ntohs(req->header.length) < sizeof(MSG_LOGIN_REQ) - MSG_HEADER_LEN)
    {
        LOG_ERROR("login message length not enough");
        return -1;
    }

    if(!session)
    {
        LOG_FATAL("session ptr null");
        return -1;
    }

    char imei[IMEI_LENGTH + 1] = {0};
    memcpy(imei, req->imei, IMEI_LENGTH);

    OBJECT *obj = session->obj;
    if(!obj)
    {
        LOG_INFO("device %s login", req->imei);
        obj = obj_get(imei);
        if(!obj)
        {
            obj = obj_new();

            memcpy(obj->IMEI, imei, IMEI_LENGTH + 1);

            obj_add(obj);
        }

        session->obj = obj;
        session_add(session);
        obj->session = session;
    }
    else
    {
        LOG_INFO("device imei(%s) already login", imei);
    }

    MSG_LOGIN_RSP *rsp = alloc_simcomMsg(req->header.cmd, sizeof(MSG_LOGIN_RSP));//login rsp
    if(!rsp)
    {
        LOG_ERROR("insufficient memory");
        return -1;
    }

    LOG_INFO("send login rsp");
    simcom_sendMsg(rsp, sizeof(MSG_LOGIN_RSP), session);
    return 0;
}

static int simcom_ping(const void *msg, SESSION *session)
{
    const MSG_PING_REQ *req = (const MSG_PING_REQ *)msg;
    if(ntohs(req->header.length) < sizeof(MSG_PING_REQ) - MSG_HEADER_LEN)
    {
        LOG_ERROR("login message length not enough");
        return -1;
    }

    LOG_INFO("device %s ping", req->imei);
    return 0;
}

static int simcom_setServer(const void *msg, SESSION *session)
{
    return 0;
}

static int simcom_data(const void *msg, SESSION *session)
{
    const MSG_DATA_REQ *req = (const MSG_DATA_REQ *)msg;
    if(!req)
    {
        LOG_ERROR("msg handle empty");
        return -1;
    }

    if(ntohs(req->header.length) < sizeof(MSG_DATA_REQ) - MSG_HEADER_LEN)
    {
        LOG_ERROR("collector_info message length not enough");
        return -1;
    }
    LOG_INFO("device(%s) timestamp(%d)", req->imei, req->timestamp);
    LOG_INFO("timestamp: %d", req->timestamp);
    LOG_INFO("latitude: %f", req->latitude);
    LOG_INFO("longitude: %f", req->longitude);
    LOG_HEX(req->data, AD_DATA_LEN);
    return 0;
}

static int simcom_dynamic(const void *msg, SESSION *session)
{
    const MSG_DATADYNAMIC_REQ *req = (const MSG_DATADYNAMIC_REQ *)msg;
    if(!req)
    {
        LOG_ERROR("msg handle empty");
        return -1;
    }

    if(ntohs(req->header.length) < sizeof(MSG_DATADYNAMIC_REQ) - MSG_HEADER_LEN)
    {
        LOG_ERROR("collector_info message length not enough");
        return -1;
    }
    LOG_INFO("device %s dynamic data", req->imei);
    LOG_HEX(req->data, AD_DATADYNAMIC_LEN);
    return 0;
}

static int simcom_set(const void *msg, SESSION *session)
{
    const MSG_SET_RSP *req = (const MSG_SET_RSP *)msg;
    if(!req)
    {
        LOG_ERROR("msg handle empty");
        return -1;
    }

    if(ntohs(req->header.length) < sizeof(MSG_SET_RSP) - MSG_HEADER_LEN)
    {
        LOG_ERROR("collector_info message length not enough");
        return -1;
    }
    LOG_HEX(req->data, ntohs(req->header.length));
    return 0;
}

static MSG_PROC_MAP msgProcs[] =
{
    {CMD_LOGIN,               simcom_login},
    {CMD_PING,                simcom_ping},
    {CMD_SERVER,              simcom_setServer},
    {CMD_DATA,                simcom_data},
    {CMD_DYNAMIC,             simcom_dynamic},
    {CMD_SET,                 simcom_set}
};

static int handle_one_msg(const void *m, SESSION *ctx)
{
    const MSG_HEADER *msg = (const MSG_HEADER *)m;

    for (size_t i = 0; i < sizeof(msgProcs) / sizeof(msgProcs[0]); i++)
    {
        if (msgProcs[i].cmd == msg->cmd)
        {
            MSG_PROC pfn = msgProcs[i].pfn;
            if (pfn)
            {
                return pfn(msg, ctx);
            }
        }
    }

    return -1;
}

int handle_simcom_msg(const char *m, size_t msgLen, void *arg)
{
    const MSG_HEADER *msg = (const MSG_HEADER *)m;

    if(msgLen < MSG_HEADER_LEN)
    {
        LOG_ERROR("message length not enough: %zu(at least(%zu)", msgLen, sizeof(MSG_HEADER));

        return -1;
    }

    size_t leftLen = msgLen;
    while(leftLen >= ntohs(msg->length) + MSG_HEADER_LEN)
    {
        unsigned short signature = (unsigned)ntohs(msg->signature);
        if(signature != START_FLAG)
        {
            LOG_ERROR("receive message header signature error:%x", signature);
            return -1;
        }
        handle_one_msg(msg, (SESSION *)arg);
        leftLen = leftLen - MSG_HEADER_LEN - ntohs(msg->length);
        msg = (const MSG_HEADER *)(m + msgLen - leftLen);
    }
    return 0;
}
