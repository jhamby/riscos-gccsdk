/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/utime.h,v $
 * $Date: 2000/07/15 14:52:12 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* POSIX Standard: 5.6.6 Set File Access and Modification Times <utime.h> */

#ifndef	__UTIME_H
#define	__UTIME_H	1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Structure describing file times.  */
struct utimbuf
  {
    __time_t actime;		/* Access time.  */
    __time_t modtime;		/* Modification time.  */
  };

/* Set the access and modification times of FILE to those given in TIMES.
   If TIMES is NULL, set them to the current time.  */
extern int utime (const char *file, const struct utimbuf *times);

#ifdef __cplusplus
}
#endif

#endif
