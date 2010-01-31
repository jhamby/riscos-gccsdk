/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __SYS_TIMEB_H
#define __SYS_TIMEB_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#define __need_time_t
#include <time.h>

__BEGIN_DECLS

/* Structure returned by the `ftime' function.  */

struct timeb
  {
    time_t time;		/* Seconds since epoch, as from `time'.  */
    unsigned short int millitm;	/* Additional milliseconds.  */
    short int timezone;		/* Minutes west of GMT.  */
    short int dstflag;		/* Nonzero if Daylight Savings Time used.  */
  };

/* Fill in TIMEBUF with information about the current time.  */
extern int ftime (struct timeb *__timebuf) __THROW;

__END_DECLS

#endif	/* sys/timeb.h */
