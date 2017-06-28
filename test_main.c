/*
 * =====================================================================================
 *       Copyright (c), 2013-2020, HWT.
 *       Filename:  test_main.c
 *
 *    Description:  
 *         Others:
 *
 *        Version:  1.0
 *        Created:  Saturday, June 04, 2016 08:49:18 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joy. Hou (hwt), houwentaoff@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#define _GNU_SOURCE
#include <limits.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/syscall.h>
#include <assert.h>
#include "include.h"
#include "debug.h"
#include "log.h"

#define LOG_PATH            "./log/test.log"            /*  */

int main ( int argc, char *argv[] )
{
#if 0
    int pipestdout[2], pipefile[2];
#endif
    int ret;
    INIT_G();
    G.logFileSize = _M(1);// _M(1);
    G.logFileRotate = 9;
    G.logFile.path = LOG_PATH;
    log_init(LOG_PATH);
#if 0 
    ret = pipe(pipestdout);
    assert(ret!=-1);
    ret = pipe(pipefile);
    assert(ret!=-1);
#endif
    while (1)
    {
        ut_dbg("hello\n");
#if 0
//        int size = syscall(__NR_tee, 1, G.logFile.fd, INT_MAX, SPLICE_F_NONBLOCK);
        ret=splice(STDOUT_FILENO,NULL,pipestdout[1],NULL,32768,SPLICE_F_MORE
                                                |SPLICE_F_MOVE);
        assert(ret!=-1);
        ret=tee(pipestdout[0],pipefile[1],32768,SPLICE_F_NONBLOCK);
        assert(ret!=-1);
        ret=splice(pipefile[0],NULL, G.logFile.fd,NULL,32768,SPLICE_F_MORE);//|SPLICE_F_MOVE);
        assert(ret!=-1);
        ret=splice(pipestdout[0],NULL,STDOUT_FILENO,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
        assert(ret!=-1);
//        int size = tee(1, G.logFile.fd, INT_MAX, SPLICE_F_NONBLOCK);
//        perror("tee\n");
//        printf("size[%d]\n", size);
#endif
        fflush(NULL);
        sleep(1);
    }
    return 0;
}

