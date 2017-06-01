/*
 * timer.h
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include <event2/event.h>

struct event* timer_newLoop(struct event_base *base, const struct timeval *timeout, event_callback_fn cb, void *arg);
struct event* timer_newOnce(struct event_base *base, const struct timeval *timeout, event_callback_fn cb, void *arg);

void timer_react(struct event *ev, struct timeval *timeout);

#endif /* SRC_TIMER_H_ */
