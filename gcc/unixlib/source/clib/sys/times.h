/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/times.h,v $
 * $Date: 2000/07/15 14:52:16 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* POSIX Standard 4.5.2: Process Times <sys/times.h>.  */

#ifndef __SYS_TIMES_H
#define __SYS_TIMES_H

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define HZ 100

/* Structure describing CPU time used by a process and its children.  */

struct tms
{
  __clock_t tms_utime;     /* user time */
  __clock_t tms_stime;     /* system time */
  __clock_t tms_cutime;    /* user time of dead children */
  __clock_t tms_cstime;    /* system time of dead children */
};

/* Store the CPU time used by this process and all its dead children
   in buffer.  */
__clock_t times (struct tms *tmsp);

#ifdef __cplusplus
}
#endif

#endif
