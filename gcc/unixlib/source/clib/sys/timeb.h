/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/timeb.h,v $
 * $Date: 2001/09/14 14:01:17 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __SYS_TIMEB_H
#define __SYS_TIMEB_H 1

#define __need_time_t
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Structure returned by the `ftime' function.  */

struct timeb
  {
    time_t time;		/* Seconds since epoch, as from `time'.  */
    unsigned short int millitm;	/* Additional milliseconds.  */
    short int timezone;		/* Minutes west of GMT.  */
    short int dstflag;		/* Nonzero if Daylight Savings Time used.  */
  };

/* Fill in TIMEBUF with information about the current time.  */
extern int ftime (struct timeb *__timebuf);

#ifdef __cplusplus
}
#endif

#endif	/* sys/timeb.h */
