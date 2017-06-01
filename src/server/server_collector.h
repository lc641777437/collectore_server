/*
 * server_simcom.h
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */

#ifndef SRC_SERVER_SIMCOM_H_
#define SRC_SERVER_SIMCOM_H_

#include <event2/event.h>

struct evconnlistener* server_liquid(struct event_base* base, int port);

#endif /* SRC_SERVER_SIMCOM_H_ */
