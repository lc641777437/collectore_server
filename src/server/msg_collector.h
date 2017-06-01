/*
 * msg_simcon.h
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */

#ifndef SRC_MSG_SIMCOM_H_
#define SRC_MSG_SIMCOM_H_

#include "protocol.h"

MSG_HEADER* alloc_simcomMsg(char cmd, size_t length);

void free_simcom_msg(void* msg);

char get_msg_cmd(void *msg);



#endif /* SRC_MSG_SIMCOM_H_ */
