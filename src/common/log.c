/*
 * log.c
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */

#include <string.h>

#include "log.h"

#ifdef WITH_CATEGORY

zlog_category_t* cat[MOD_MAX];

static const char* all_modules[] =
{
		"MAIN",
		"SERVER_MC",
		"SCH_MC",
		"PROC_MC",
		"FSM",
		"HTTP",
		"MQTT",
		"GIZWITS_REQ",
		"GIZWITS_RSP",
};
#else
zlog_category_t *cat;
#endif

int log_init(const char *conf)
{
    int rc;

    rc = zlog_init(conf);
    if (rc) {
    	printf("%s:%d log init failed\n", __FILE__, __LINE__);
    	return -1;
    }

#ifdef WITH_CATEGORY
    memset(cat, 0, sizeof(cat));

    for (size_t i = 0; i < sizeof(all_modules) / sizeof(const char*); i++)
    {
    	zlog_category_t* c = zlog_get_category(all_modules[i]);
        if (!c)
        {
        	printf("get category %s fail\n", all_modules[i]);

        	zlog_fini();
        	return -2;
        }
        cat[i] = c;
    }
#else
    cat = zlog_get_category("my_cat");

    if (!cat)
    {
		printf("get cat fail\n");

		zlog_fini();
		return -2;
    }
#endif

    return 0;
}
