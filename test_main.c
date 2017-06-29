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

#include <libgen.h>
#include <getopt.h>

#include <stdlib.h>
#include <sys/resource.h>

#define LOG_PATH            "./log/test.log"            /*  */
#define LIBLORAGW_VERSION   "1.0.1"            /*  */
static const char* program_name;

static const char *short_options = "hvl:";
static struct option long_options[] = {
    {"help", 0, NULL, 'h'}, 
    {"version", 0, NULL, 'v'}, 
    {"core", 0, NULL, 'c'},     
    {"log", 1, NULL, 'l'}, 
    {0, 0, 0, 0}    
};
static struct
{
    char version[128];//no default
    char log_path[128];
    int log_flag;
    int coredump;    
}loragw_params;
int gen_core()
{
#define  EnableCoreDumps()\
{\
    struct rlimit   limit;\
    limit.rlim_cur = RLIM_INFINITY;\
    limit.rlim_max = RLIM_INFINITY;\
    setrlimit(RLIMIT_CORE, &limit);\
}    
    EnableCoreDumps();
    mkdir("./cores", 0775);//mkdir -p ../cores
    system("sysctl -w kernel.core_pattern=./cores/core.%e-%p-%t");
    return 0;
}
static void print_usage(FILE *f, int exit_code)
{
    fprintf(f, "\nUsage: %s [options] ...\n", 
        program_name);
    fprintf(f,
            "\t -h  --help         \n"
            "\t -v  --version       \n"
            "\t -l  --log    default \"./log/test.log\"\n"
            "\t -c  --core   gen coredump file    \n"            
            "\t example:%s  -l /mnt/data/log/lora.log \n",
            program_name
            );
    exit(exit_code);
}
static int init_params(int argc, char **argv)
{
    int ch;
    int option_index = 0;

    opterr = 0;
    program_name = argv[0];

    if (argc < 1)
    {
        return -1;
    }
    while ((ch = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
            switch (ch) {
            case 'h':
                print_usage(stderr, 0);
                break;
            case 'v':
                printf("%s\n", LIBLORAGW_VERSION);
                exit(0);
                break;
            case 'l':
                strcpy(loragw_params.log_path, optarg);
                loragw_params.log_flag = 1;
                break;
            case 'c':
                loragw_params.coredump = 1;
                break;                
            }
    }
#if 0    
    if (loragw_params.log_flag == 1)
    {
        strcpy(loragw_params.log_path, DEFAULT_LOG_PATH);
    }
#endif    
    return 0;
}

int main ( int argc, char *argv[] )
{
#if 0
    int pipestdout[2], pipefile[2];
#endif
 
	if (init_params(argc, argv)<0)
    {
        print_usage(stderr, 1);
        return -1;
    }
    if (loragw_params.coredump == 1)
    {
        gen_core();
    }
    if (loragw_params.log_flag == 1)
    {
        INIT_G();
        G.logFileSize = _M(1);// _M(1);
        G.logFileRotate = 9;
        G.logFile.path = loragw_params.log_path;
        log_init(loragw_params.log_path);
    }
    
    int ret;

#if 0 
    ret = pipe(pipestdout);
    assert(ret!=-1);
    ret = pipe(pipefile);
    assert(ret!=-1);
#endif
    while (1)
    {
        if (loragw_params.log_flag == 1)
        {
            ut_dbg("hello\n");
        }
        else
        {
            printf("hello\n");
        }
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

