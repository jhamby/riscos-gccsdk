/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/timeb,v $
 * $Date: 1997/10/09 19:59:57 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __SYS_TIMEB_H
#define __SYS_TIMEB_H 1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Structure returned by the `ftime' function.  */

struct timeb
  {
    __time_t time;		/* Seconds since epoch, as from `time'.  */
    unsigned short int millitm;	/* Additional milliseconds.  */
    short int timezone;		/* Minutes west of GMT.  */
    short int dstflag;		/* Nonzero if Daylight Savings Time used.  */
  };

/* Fill in TIMEBUF with information about the current time.  */
extern int ftime (struct timeb *timebuf);

#ifdef __cplusplus
}
#endif

#endif	/* sys/timeb.h */
