/*
*main.c
*
*  Created on: 2016-11-8
*      Author: lc
*/
#include <event2/event.h>
#include <curl/curl.h>
#include <signal.h>
#include <openssl/ssl.h>
#include <event2/listener.h>

#include "log.h"
#include "version.h"
#include "server_collector.h"
#include "object.h"
#include "port.h"
#include "session.h"

static void signal_cb(evutil_socket_t fd __attribute__((unused)), short what __attribute__((unused)), void *arg)
{
    struct event_base *base = arg;

    LOG_WARN("server being stopped");

    event_base_loopbreak(base);
}

int main(int argc, char **argv)
{
    int rc = log_init("../conf/log.conf");
    if (rc)
    {
        LOG_ERROR("log initial failed: rc=%d", rc);
    	return rc;
    }

    int collector_port = PORT_LIQUID_LEVEL;

    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc >= 2)
    {
    	char* strPort = argv[1];
    	int num = atoi(strPort);
    	if (num)
    	{
    		collector_port = num;
    	}
    }

    LOG_DEBUG("liquid level server %s start :%d", VERSION_STR, PORT_LIQUID_LEVEL);

    struct event_base *base = event_base_new();
    if (!base)
    {
        return 1;
    }


    struct event *evTerm = evsignal_new(base, SIGTERM, signal_cb, base);
    if (!evTerm || evsignal_add(evTerm, NULL) < 0)
    {
        LOG_ERROR("can't create SIGTERM event");
    }

    struct event *evInt = evsignal_new(base, SIGINT, signal_cb, base);
    if (!evInt || evsignal_add(evInt, NULL) < 0)
    {
        LOG_ERROR("can't create SIGINT event");
    }

    obj_table_initial();
    session_table_initial();

    struct evconnlistener *listener_liquid = server_liquid(base, collector_port);
    if (listener_liquid)
    {
        LOG_INFO("start collector server successfully at port:%d", collector_port);
    }
    else
    {
        LOG_ERROR("start collector server failed at port:%d", collector_port);
        return 2;
    }

    LOG_INFO("start the event loop");
    event_base_dispatch(base);

    LOG_INFO("stop collector server...");

    evconnlistener_free(listener_liquid);

    evsignal_del(evTerm);
    evsignal_del(evInt);

    event_base_free(base);

    session_table_destruct();
    obj_table_destruct();

    zlog_fini();

    return 0;
}
