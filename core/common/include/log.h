#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#ifndef LOGGER_STDOUT
#define LOGGER_STDOUT      0
                            /**!< Log output to stdout(=1) or file(=0) */
#endif /*LOGGER_STDOUT*/

#define LOGGER_DIR         "/home/app/monkey"
                           /**!< Directory name of Log destination */

/* Maximum lines/files */
#define LOGGER_MAXLINES    20000
                            /**!< Maximum output lines of log file  */
#define LOGGER_MAXFILES    6
                            /**!< Maximum number of the log file    */

/* Log output level */
#define LOGGER_LVL_PRF     0x0200  /**!< Performance */
#define LOGGER_LVL_TRA     0x0100  /**!< Trace       */
#define LOGGER_LVL_DBG     0x0080  /**!< Debug write */
#define LOGGER_LVL_INF     0x0040  /**!< Information */
#define LOGGER_LVL_WRN     0x0010  /**!< Warning     */
#define LOGGER_LVL_CRI     0x0008  /**!< Critical    */
#define LOGGER_LVL_ERR     0x0004  /**!< Error       */

/* Log output flush */
#define LOGGER_FLUSH       0x4000  /**!< Log outout with log flush     */
#define LOGGER_NOFLUSH     0x2000  /**!< Log output without log flush  */

#define LOGGER_ENABLE

#ifdef LOGGER_ENABLE

#define LOGGER_PRF(fmt,...)                        \
{                                               \
    logger_print(LOGGER_LVL_PRF,              \
                  "%s> " fmt " (%s,%s:%d)\n",   \
                  logger_get_str_cur_time("PRF"),  \
                  ##__VA_ARGS__,                \
                  __func__,                     \
                  __FILE__,                     \
                  __LINE__);                    \
}

#define LOGGER_TRA(fmt,...)                        \
{                                               \
    logger_print(LOGGER_LVL_TRA,              \
                  "%s> " fmt " (%s,%s:%d)\n",   \
                  logger_get_str_cur_time("TRA"),  \
                  ##__VA_ARGS__,                \
                  __func__,                     \
                  __FILE__,                     \
                  __LINE__);                    \
}

#define LOGGER_DBG(fmt,...)                        \
{                                               \
    logger_print(LOGGER_LVL_DBG,              \
                  "%s> " fmt " (%s,%s:%d)\n",   \
                  logger_get_str_cur_time("DBG"),  \
                  ##__VA_ARGS__,                \
                  __func__,                     \
                  __FILE__,                     \
                  __LINE__);                    \
}

#define LOGGER_INF(fmt,...)                        \
{                                               \
    logger_print(LOGGER_LVL_INF,              \
                  "%s> " fmt " (%s,%s:%d)\n",   \
                  logger_get_str_cur_time("INF"),  \
                  ##__VA_ARGS__,                \
                  __func__,                     \
                  __FILE__,                     \
                  __LINE__);                    \
}

#define LOGGER_WRN(fmt,...)                        \
{                                               \
    logger_print(LOGGER_LVL_WRN,              \
                  "%s> " fmt " (%s,%s:%d)\n",   \
                  logger_get_str_cur_time("WRN"),  \
                  ##__VA_ARGS__,                \
                  __func__,                     \
                  __FILE__,                     \
                  __LINE__);                    \
}

#define LOGGER_CRI(fmt,...)                        \
{                                               \
    logger_print(LOGGER_LVL_CRI,              \
                  "%s> " fmt " (%s,%s:%d)\n",   \
                  logger_get_str_cur_time("CRI"),  \
                  ##__VA_ARGS__,                \
                  __func__,                     \
                  __FILE__,                     \
                  __LINE__);                    \
}

#define LOGGER_ERR(fmt,...)                        \
{                                               \
    logger_print(LOGGER_LVL_ERR,              \
                  "%s> " fmt " (%s,%s:%d)\n",   \
                  logger_get_str_cur_time("ERR"),  \
                  ##__VA_ARGS__,                \
                  __func__,                     \
                  __FILE__,                     \
                  __LINE__);                    \
}

#else
#define LOGGER_PRF(fmt,...)
#define LOGGER_TRA(fmt,...)
#define LOGGER_DBG(fmt,...)
#define LOGGER_INF(fmt,...)
#define LOGGER_WRN(fmt,...)
#define LOGGER_CRI(fmt,...)
#define LOGGER_ERR(fmt,...)
#endif


void logger_print(int level, const char *fmt, ...);
void logger_open(const char *prog);
void logger_close(void);
void logger_flush(void);
char * logger_get_str_cur_time(const char *level);
void logger_set_level(int level);

#ifdef __cplusplus
}
#endif

#endif // __LOG_H__
