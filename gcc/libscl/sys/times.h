#ifndef __SYS_TIMES_H
#define __SYS_TIMES_H

#define __need_clock_t
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Structure describing CPU time used by a process and its children.  */

struct tms
{
  clock_t tms_utime;     /* user time */
  clock_t tms_stime;     /* system time */
  clock_t tms_cutime;    /* user time of dead children */
  clock_t tms_cstime;    /* system time of dead children */
};

/* Store the CPU time used by this process and all its dead children
   in buffer.  */
extern clock_t times (struct tms *__tmsp);

#ifdef __cplusplus
}
#endif

#endif
