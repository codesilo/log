/*
 * =====================================================================================
 *       Copyright (c), 2013-2020, Sobey.
 *       Filename:  include.h
 *
 *    Description:  
 *         Others:
 *
 *        Version:  1.0
 *        Created:  2015/9/14 10:13:17
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Sean. Hou (hwt), houwentaoff@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <time.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define _M(x)                   (x*(1024*1024))/*  x M bytes*/

#define G (*ptr_to_globals)

#define INIT_G()  do { \
    ptr_to_globals = (struct globals *)&globals; \
    memset((void *)&globals, 0, sizeof(globals)); \
}while (0)

struct globals;
extern struct globals *ptr_to_globals;
extern struct globals globals;
typedef struct logFile_t{
    const char *path;
    int fd;
    unsigned  size;
    unsigned int isRegular;
}logFile_t;
struct globals{
    /* log */
    logFile_t logFile;
    /* max size of file before rotation */ 
    unsigned logFileSize;
    /* number of rotated message files */  
    unsigned logFileRotate;
    time_t lastLogTime;
    /* exe path */
    const char *exe;
    /*  cwd path */
    const char *cwd;
    /* root dir */
    const char *root;
};
#endif
