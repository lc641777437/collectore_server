/*
 * msg_proc_simcom.h
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */

#ifndef SRC_MSG_PROC_COLLECTOR_H_
#define SRC_MSG_PROC_COLLECTOR_H_

#include <stdio.h>
#include "session.h"

int handle_simcom_msg(const char* m, size_t msgLen, void* arg);

#endif /* SRC_MSG_PROC_SIMCOM_H_ */
