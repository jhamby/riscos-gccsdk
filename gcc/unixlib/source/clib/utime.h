/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/utime.h,v $
 * $Date: 2001/09/14 14:01:17 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* POSIX Standard: 5.6.6 Set File Access and Modification Times <utime.h> */

#ifndef	__UTIME_H
#define	__UTIME_H  1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_time_t
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Structure describing file times.  */
struct utimbuf
  {
    time_t actime;		/* Access time.  */
    time_t modtime;		/* Modification time.  */
  };

/* Set the access and modification times of FILE to those given in TIMES.
   If TIMES is NULL, set them to the current time.  */
extern int utime (const char *__file, const struct utimbuf *__times);

#ifdef __cplusplus
}
#endif

#endif
