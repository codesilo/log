/*
 * =====================================================================================
 *       Copyright (c), 2013-2020, HWT.
 *       Filename:  log.c
 *
 *    Description:  
 *         Others:
 *
 *        Version:  1.0
 *        Created:  06/02/2016 09:53:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Joy. Hou (hwt), houwentaoff@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "include.h"
#include "debug.h"
#include <unistd.h>
#include <stdlib.h>

#define ENABLE_FEATURE_ROTATE_LOGFILE  1
#define DEV_CONSOLE "/dev/console"

#ifndef WIN32
#define TIMEZONE_OFFSET(foo) foo->tm_gmtoff
#else
#define TIMEZONE_OFFSET(foo) (8*60*60)
#endif

int use_localtime = 1 ;
const char month_tab[49] =
    "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ";
    const char day_tab[] = "Sun,Mon,Tue,Wed,Thu,Fri,Sat,";

struct globals *ptr_to_globals;
struct globals globals;
static logFile_t *log_file=NULL;


int log_init(const char *path)
{
    char cmdBuf[512]={0};
    char dirName[512]={0};

    G.logFile.path = path;
    strcpy(dirName, path);
    dirname(dirName);
    log_file = &(G.logFile);
    
    sprintf(cmdBuf, "mkdir -p %s", dirName);
    system(cmdBuf);
    if ((log_file->fd = open(G.logFile.path, O_APPEND|O_CREAT|O_WRONLY, 0666)) < 0)
    {
        perror("open log file fail\n");
        return -1;
    }
    dup2(log_file->fd, 2);
    dup2(log_file->fd, 1);
//    dup2(log_file->fd, infd);

    return 0;
}
/*  try to open up the specified device */
int device_open(const char *device, int mode)
{
    int m, f, fd;

    m = mode | O_NONBLOCK;

    /*  Retry up to 5 times */
    /*  TODO: explain why it can't be considered insane */
    for (f = 0; f < 5; f++) {
        fd = open(device, m, 0600);
        if (fd >= 0)
            break;
    }
    if (fd < 0)
        return fd;
    /*  Reset original flags. */
    if (m != mode)
        fcntl(fd, F_SETFL, mode);
    return fd;
}

char *get_commonlog_time(void)
{
    struct tm *t;
    char *p;
    unsigned int a;
    static char buf[30];
    int time_offset;
    time_t current_time;
    time(&current_time);
    if (G.logFile.fd >= 0)
    {
        if (G.lastLogTime != current_time)
        {
            G.lastLogTime = current_time;
            close(globals.logFile.fd);
            goto reopen;
        }
    }
    else
    {
reopen:
        if (G.logFile.fd < 0) {
            /*  cannot open logfile? - print to /dev/console then */
            int fd = device_open(DEV_CONSOLE, O_WRONLY | O_NOCTTY | O_NONBLOCK);
            if (fd < 0)
                fd = 2; /*  then stderr, dammit */
//            full_write(fd, msg, len);
            if (fd != 2)
                close(fd);
            return "";
        }
        log_file->fd = open(log_file->path, O_WRONLY | O_CREAT
                            | O_NOCTTY | O_APPEND | O_NONBLOCK,
                           0666);
        dup2(log_file->fd, 1);
        dup2(log_file->fd, 2);
#if ENABLE_FEATURE_ROTATE_LOGFILE
        struct stat statf;
        G.logFile.isRegular = (fstat(G.logFile.fd, &statf) == 0 
            && S_ISREG(statf.st_mode)
            );
        G.logFile.size = statf.st_size;
#endif
    }
#if ENABLE_FEATURE_ROTATE_LOGFILE
    if (G.logFileSize && G.logFile.isRegular && G.logFile.size > G.logFileSize)
    {
        if (G.logFileRotate)/*always 0...99 */
        {
#if 1
			int i = strlen(log_file->path) + 3 + 1;
			char oldFile[i];
			char newFile[i];
			i = G.logFileRotate - 1;
			/* rename: f.8 -> f.9; f.7 -> f.8; ... */
			while (1) {
				sprintf(newFile, "%s.%d", log_file->path, i);
				if (i == 0) break;
				sprintf(oldFile, "%s.%d", log_file->path, --i);
				/* ignore errors - file might be missing */
				rename(oldFile, newFile);
			}
			/* newFile == "f.0" now */
			rename(log_file->path, newFile);
			/* Incredibly, if F and F.0 are hardlinks, POSIX
			 * _demands_ that rename returns 0 but does not
			 * remove F!!!
			 * (hardlinked F/F.0 pair was observed after
			 * power failure during rename()).
			 * Ensure old file is gone:
			 */
			unlink(log_file->path);
#ifdef SYSLOGD_WRLOCK
			fl.l_type = F_UNLCK;
			fcntl(log_file->fd, F_SETLKW, &fl);
#endif
			close(log_file->fd);
			goto reopen;
#endif
        }
		ftruncate(G.logFile.fd, 0);
    }
#endif
    if (use_localtime) {
        t = localtime(&current_time);
        time_offset = TIMEZONE_OFFSET(t);
    } else {
        t = gmtime(&current_time);
        time_offset = 0;
    }

    p = buf + 29;
    *p-- = '\0';
    *p-- = ' ';
    *p-- = ']';
    a = abs(time_offset / 60);
    *p-- = '0' + a % 10;
    a /= 10;
    *p-- = '0' + a % 6;
    a /= 6;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = (time_offset >= 0) ? '+' : '-';
    *p-- = ' ';

    a = t->tm_sec;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_min;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_hour;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = 1900 + t->tm_year;
    while (a) {
        *p-- = '0' + a % 10;
        a /= 10;
    }
    /* p points to an unused spot */
    *p-- = '/';
    p -= 2;
    memcpy(p--, month_tab + 4 * (t->tm_mon), 3);
    *p-- = '/';
    a = t->tm_mday;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p = '[';
    return p;                   /* should be same as returning buf */
}



