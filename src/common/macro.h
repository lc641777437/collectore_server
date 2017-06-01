/*
 * macro_mc.h
 *
 *  Created on: 2016-11-8
 *      Author: lc
 */

#ifndef SRC_MACRO_H_
#define SRC_MACRO_H_

#define IMEI_LENGTH (15)
#define CCID_LENGTH (20)
#define IMSI_LENGTH (15)
#define MAX_PWD_LEN (16)
#define MAC_MAC_LEN (6)
#define CELL_NUM (7)


enum
{
    ObjectType_null = 0,
    ObjectType_tk115 = 1,
    ObjectType_simcom = 2,
    ObjectType_collector = 3,
};

#endif /* SRC_MACRO_H_ */
