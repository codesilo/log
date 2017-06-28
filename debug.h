/*
 * =====================================================================================
 *       Copyright (c), 2013-2020, HWT.
 *       Filename:  debug.h
 *
 *    Description:  
 *         Others:
 *
 *        Version:  1.0
 *        Created:  2015/9/14 10:02:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sean. Hou (hwt), houwentaoff@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <time.h>
#include "log.h"

#define    UT_DEBUG                   1/*debug switch*/
#define    UT_ERROR                   1/*debug switch*/

#if     UT_DEBUG
#define ut_dbg(fmt, args...)                                                                \
do                                                                                          \
{                                                                                           \
    printf("%s:", get_commonlog_time());                                                    \
    printf("[update dbg]:%s:%d: "fmt"\n", __FILE__, __LINE__, ##args);                          \
}while(0)    /* */  

#else
#define ut_dbg(...)
#endif

#if     UT_ERROR
#define ut_err(fmt, args...)                                                                \
do                                                                                          \
{                                                                                           \
    printf("%s:", get_commonlog_time());                                                    \
    printf("[update err]:%s:%d: "fmt"\n", __FILE__, __LINE__, ##args);                          \
}while(0)
#else
#define ut_err(...)
#endif

#endif

