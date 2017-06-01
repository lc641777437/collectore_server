/*
 * log.h
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */

#ifndef SRC_LOG_H_
#define SRC_LOG_H_

#include <zlog.h>
#ifdef __cplusplus
    extern "C"{
#endif

#ifdef WITH_CATEGORY

enum
{
	MOD_MAIN,
	MOD_SERVER_MC,
	MOD_SCH_MC,
	MOD_PROC_MC,
	MOD_FSM,
	MOD_HTTP,
	MOD_MQTT,
	MOD_GIZWITS_REQ,
	MOD_GIZWITS_RSP,
	MOD_MAX
};
extern zlog_category_t* cat[];

#elif defined(WITH_LOG)

extern zlog_category_t* cat;

#define LOG_MSG(buf, buf_len) \
	hzlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, ZLOG_LEVEL_FATAL, buf, buf_len)

#define LOG_DEBUG(...) \
	zlog(cat, __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__)

#define LOG_INFO(...) \
	zlog(cat, __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_INFO, __VA_ARGS__)

#define LOG_WARN(...) \
	zlog(cat, __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_WARN, __VA_ARGS__)

#define LOG_ERROR(...) \
	zlog(cat, __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_ERROR, __VA_ARGS__)

#define LOG_FATAL(...) \
	zlog(cat, __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_FATAL, __VA_ARGS__)

#define LOG_HEX(buf, buf_len) \
	hzlog(cat, __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, ZLOG_LEVEL_DEBUG, buf, buf_len)

#else

#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
#define LOG_HEX(buf, buf_len)

#endif

int log_init(const char *conf);
#ifdef __cplusplus
    }
#endif

#endif /* SRC_LOG_H_ */
