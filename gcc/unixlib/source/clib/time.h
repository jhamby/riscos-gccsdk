/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/time.h,v $
 * $Date: 2003/06/07 02:30:21 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

/* ISO C99 Standard: 7.23 Date and Time <time.h>.  */

#ifndef __TIME_H

#if (! defined __need_time_t && ! defined __need_clock_t \
    && ! defined __need_timespec && ! defined __need_clockid_t \
    && ! defined __need_timer_t)
#define __TIME_H

#define __need_time_t
#define __need_clock_t
#define __need_timespec
#define __need_clockid_t
#define __need_timer_t

#define __need_size_t
#define __need_NULL
#include <stddef.h>
#endif


#if !defined __clock_t_defined && defined __need_clock_t
#define __clock_t_defined 1
#include <unixlib/types.h>
typedef __clock_t clock_t;  /* Data type used to represent clock ticks.  */
#endif
#undef __need_clock_t

#if !defined __clockid_t_defined && defined __need_clockid_t
#define __clockid_t_defined 1
#include <unixlib/types.h>
typedef __clockid_t clockid_t;  /* Data type used to represent clock ticks.  */
#endif
#undef __need_clockid_t

#if !defined __time_t_defined && defined __need_time_t
#define __time_t_defined 1
#include <unixlib/types.h>
/* The data type used to represent calendar time. It represents
   the number of seconds elapsed since 00:00:00 on 1 January, 1970
   Universal Time Coordinated.  */
typedef __time_t time_t;
#endif
#undef __need_time_t

#if !defined __timer_t_defined && defined __need_timer_t
#define __timer_t_defined 1
#include <unixlib/types.h>
/* The data type used to represent calendar time. It represents
   the number of seconds elapsed since 00:00:00 on 1 January, 1970
   Universal Time Coordinated.  */
typedef __timer_t timer_t;
#endif
#undef __need_timer_t

#if !defined __timespec_defined && defined __need_timespec
#define __timespec_defined 1

/* POSIX.1b structure for a time value.  This is like a `struct timeval' but
   has nanoseconds instead of microseconds.  */
struct timespec
{
  long int tv_sec;  /* Seconds.  */
  long int tv_nsec; /* Nanoseconds.  */
};
#endif
#undef __need_timespec


#ifdef __TIME_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

__BEGIN_DECLS

/* The number of clock ticks per second measured by the clock function.  */
#define CLOCKS_PER_SEC 100

/* Obsolete name for CLOCKS_PER_SEC.  */
#define CLK_TCK CLOCKS_PER_SEC

/* Return the elapsed processor time.  */
extern clock_t clock (void);

/* Return the current time as a value of type 'time_t'.
   If result is not NULL, the time will also be return in it. */
extern time_t time (time_t *__result);

struct tm
{
  int tm_sec; 	/* seconds (0 - 59) */
  int tm_min; 	/* minutes (0 - 59) */
  int tm_hour;	/* hours (0 - 23) */
  int tm_mday;	/* day of month (1 - 31) */
  int tm_mon; 	/* month of year (0 - 11) */
  int tm_year;	/* year - 1900 */
  int tm_wday;	/* day of week (Sunday = 0) */
  int tm_yday;	/* day of year (0 - 365) */
  int tm_isdst;	/* 1 - DST in effect,0 - not,-1 - not known */
  int tm_gmtoff;	/* offset east of UTC (GMT) in seconds */
  const char *tm_zone;	/* abbreviation of timezone name */
};

/* Convert the broken-down time value into a string in a standard
   format:  "Sat Jul 12 14:47:12 1997\n"  */
extern char *asctime (const struct tm *__brokentime);

/* Similar to asctime except that the time value is specified
   as a time_t calendar time value.  */
extern char *ctime (const time_t *__time);

/* Return the number of seconds elapsed between time1 and time 0
   as a value of type double.  */
extern double difftime (time_t __time1, time_t __time0);

/* Convert the calendar time 'time' to broken-down time,
   expressed relative to the user's specified time zone. */
extern struct tm *localtime (const time_t *__time);

/* Convert the calendar time 'time' to broken-down time,
   expressed relative to the user's specified time zone (re-entrant). */
extern struct tm *localtime_r (const time_t *__restrict __time,
			       struct tm *__restrict resultp);

/* Similar to localtime() but the broken-down time is expressed
   as UTC (GMT) rather than the local time zone.  */
extern struct tm *gmtime (const time_t *__time);

/* Similar to localtime() but the broken-down time is expressed
   as UTC (GMT) rather than the local time zone (re-entrant).  */
extern struct tm *gmtime_r (const time_t *__restrict __time,
			    struct tm *__restrict resultp);

/* Convert a broken-down time structure to a calendar time
   representation.  */
extern time_t mktime (struct tm *__brokentime);

/* Place characters into the 's' as controlled by the 'format'.
   'format' is specialised for printing components of 'brokentime'
   according to the locale currently specified for time conversion.  */
extern size_t strftime (char *__restrict __s, size_t __size,
			const char *__restrict __format,
       	      	        const struct tm *__restrict __brokentime);


/* System V compatibility.  */

/* The difference between UTC and the latest local standard time,
   in seconds west of UTC. The time is not adjusted for daylight
   saving and the sign is the reverse of tm_gmtoff.  */
extern int timezone;

/* Nonzero if daylight savings time rules apply.  */
extern int daylight;

/* POSIX extensions.  */

/* Initialise the tzname variable with the TimeZone information from
   CMOS.  */
extern void tzset (void);

/* tzname[0] is the name of the standard time zone.
   tzname[1] is the name for the time zone when daylight saving time
   is in use.  */
extern char *tzname[];

#ifdef __UNIXLIB_INTERNALS
extern struct tm __tz[1];

/* Convert broken time to 5-byte RISC OS time.  */
extern void __cvt_broken_time (const struct tm *__brokentime,
			       char *__riscos_time);

/* Common function for localtime() and gmtime().  */
extern struct tm *__calendar_convert (int __swinum, const time_t *__tp,
				      struct tm *resultp);

/* Common function for ctime() and asctime().  */
extern char *__standard_time (const char *__riscos_time);
#endif /* __UNIXLIB_INTERNALS */

__END_DECLS

#endif /* __TIME_H */
#else
#undef __need_time_t
#undef __need_clock_t
#undef __need_timespec
#undef __need_clockid_t
#undef __need_timer_t
#endif /* ! __TIME_H */
