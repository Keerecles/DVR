#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include "log.h"

/*============================================================================*/
/* global API                                                                 */
/*============================================================================*/
#if defined(__GNUC__) && __GNUC__ >= 4
#define LOGGER_API __attribute__ ((visibility("default")))
#else
#define LOGGER_API
#endif

/*========================================================================*/
/**
 *  static variable
 */
/*========================================================================*/

static int  time_zone    = 99*60*60;    /*!< local time difference(sec)       */
static int  log_level    = 0x7FFFFFFF;  /*!< output level debug log           */
static bool flush_mode   = true;        /*!< flush mode flag                  */
static bool initialized  = false;       /*!< initialized flag                 */
static FILE *log_fd      = NULL;        /*!< file descriptor of output debug log*/
static int  log_stdout   = 0;           /*!< flag for log output to stdout    */
static int  log_lines    = 0;           /*!< output lines                     */
static char log_prog[32] = {0,};        /*!< name of output source module     */


/*------------------------------------------------------------------------*/
/**
 *  @brief  printout log message
 *
 *  @param [in] level   log output level
 *  @param [in] fmt     message format(same as printf)
 *  @param [in] ...     arguments if need
 */
/*------------------------------------------------------------------------*/
LOGGER_API void
logger_print(int level, const char *fmt, ...)
{
    //do not print logs
//    return;
    char wr_filename[30];

	memset( wr_filename, 0x00 , 30 );

	snprintf(wr_filename, 30, "mlc.%d",
            getpid());

    if (!(log_level & level)) {
        return;
    }
    va_list     list;

    if (NULL == log_fd) {
        logger_open(wr_filename);
    }
#if LOGGER_STDOUT == 0
    else if (log_lines >= (LOGGER_MAXLINES-2)) {
        if (log_lines >= LOGGER_MAXLINES)  {
            logger_close();
            logger_open(log_prog);
        }
        else    {
            fflush(log_fd);
        }
    }
#endif /*LOGGER_STDOUT*/
    if (NULL != log_fd) {
        va_start(list, fmt);
        vfprintf(log_fd, fmt, list);
        va_end(list);
        if (flush_mode)  {
            fflush(log_fd);
        }
    }
    if (log_stdout == 0)    {
        log_lines ++;
    }
}

/*------------------------------------------------------------------------*/
/**
 *  @brief  open log file
 *
 *  @param [in] prog    program name
 */
/*------------------------------------------------------------------------*/
LOGGER_API void
logger_open(const char *prog)
{
#if LOGGER_STDOUT == 0
    int     idx;
    char    sPath[128];
    char    sPath2[128];
#endif /*LOGGER_STDOUT*/

    if (NULL != log_fd) {
        fflush(log_fd);
        if (log_stdout == 0)    {
            fclose(log_fd);
        }
    }

    log_lines = 0;

    if ((! prog) || (*prog == 0)) {
        log_stdout = 1;
        log_fd = stdout;
        log_prog[0] = 0;
        return;
    }
    else {
        strncpy(log_prog, prog, sizeof(log_prog)-1);
        log_prog[sizeof(log_prog)-1] = 0;
    }
#if LOGGER_STDOUT > 0
    log_stdout = 1;
    log_fd = stdout;
#else  /*LOGGER_STDOUT*/
    snprintf(sPath, sizeof(sPath)-1, "%s/%s.log%d",
             LOGGER_DIR, log_prog, LOGGER_MAXFILES-1);
    (void)remove(sPath);

    for (idx = (LOGGER_MAXFILES-1); idx > 0; idx--) {
        strcpy(sPath2, sPath);
        if (idx > 1)    {
            snprintf(sPath, sizeof(sPath)-1, "%s/%s.log%d",
                     LOGGER_DIR, log_prog, idx-1);
        }
        else    {
            snprintf(sPath, sizeof(sPath)-1, "%s/%s.log",
                     LOGGER_DIR, log_prog);
        }
        (void)rename(sPath, sPath2);
    }

    log_fd = fopen(sPath, "w");
    if (NULL == log_fd) {
        log_stdout = 1;
        log_fd = stdout;
    }
    else if ((initialized == false) &&
             (log_fd != stdout) && (log_fd != stderr)) {
        initialized = true;
        fflush(stdout);
        fflush(stderr);
//        stdout = log_fd;
//        stderr = log_fd;
    }
#endif /*LOGGER_STDOUT*/
}

/*------------------------------------------------------------------------*/
/**
 *  @brief  close log file
 */
/*------------------------------------------------------------------------*/
LOGGER_API void
logger_close(void)
{
#if LOGGER_STDOUT == 0
    if (NULL != log_fd) {
        fflush(log_fd);
        if (log_stdout == 0)    {
            fclose(log_fd);
        }
        log_fd = (FILE *)NULL;
    }
#endif /*LOGGER_STDOUT*/
}

/*------------------------------------------------------------------------*/
/**
 *  @brief  flush log file
 */
/*------------------------------------------------------------------------*/
LOGGER_API void
logger_flush(void)
{
    if ((NULL != log_fd) && (false == flush_mode)) {
        fflush(log_fd);
    }
}

/*------------------------------------------------------------------------*/
/**
 *  @brief   get current time string
 *
 *  @param [in] level   log level string(header of log message)
 *  @return current time string
 */
/*------------------------------------------------------------------------*/
LOGGER_API char *
logger_get_str_cur_time(const char *level)
{
    struct timeval  NowTime;
    extern long     timezone;
    static char     sBuf[28];

    gettimeofday(&NowTime, (struct timezone *)0);
    if (time_zone > (24*60*60)) {
        tzset();
        time_zone = timezone;
    }
    NowTime.tv_sec -= time_zone;

    sprintf(sBuf, "%02d:%02d:%02d.%03d[%s]@%d",
            (int)((NowTime.tv_sec/3600) % 24),
            (int)((NowTime.tv_sec/60) % 60),
            (int)(NowTime.tv_sec % 60),
            (int)NowTime.tv_usec/1000, level, getpid());

    return sBuf;
}

/*------------------------------------------------------------------------*/
/**
 *  @brief  set log output level
 *
 *  @param [in] level   log output level
 */
/*------------------------------------------------------------------------*/
LOGGER_API void
logger_set_level(int level)
{
    log_level = level & (~(LOGGER_FLUSH|LOGGER_NOFLUSH));

    if (log_level & (LOGGER_FLUSH|LOGGER_NOFLUSH)) {
        if (log_level & LOGGER_FLUSH) {
            flush_mode = true;
        }
        else    {
            flush_mode = false;
        }
    }
}
/* vim:set expandtab ts=4 sw=4: */
