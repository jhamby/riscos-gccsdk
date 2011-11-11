/*
 * ISO C99 Standard: 7.23 Date and Time <time.h>.
 * Copyright (c) 1997-2005 Nick Burrett
 * Copyright (c) 2000-2011 UnixLib Developers
 */

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
__BEGIN_NAMESPACE_STD
typedef __clock_t clock_t;  /* Data type used to represent clock ticks.  */
__END_NAMESPACE_STD
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
__BEGIN_NAMESPACE_STD
typedef __time_t time_t;
__END_NAMESPACE_STD
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
#include <features.h>
#endif

__BEGIN_DECLS

/* The number of clock ticks per second measured by the clock function.  */
#define CLOCKS_PER_SEC 100

/* Obsolete name for CLOCKS_PER_SEC.  */
#define CLK_TCK CLOCKS_PER_SEC

__BEGIN_NAMESPACE_STD

/* Return the elapsed processor time.  */
extern clock_t clock (void) __THROW;

/* Return the current time as a value of type 'time_t'.
   If result is not NULL, the time will also be return in it. */
extern time_t time (time_t *__result) __THROW;

struct tm
{
  int tm_sec; 	/* Number of seconds after the minute (0 through 59).  */
  int tm_min; 	/* Number of minutes after the hour (0 through 59).  */
  int tm_hour;	/* Number of hours past midnight (0 through 23).  */
  int tm_mday;	/* Day of the month (1 through 31).  */
  int tm_mon; 	/* Number of months since January (0 through 11).  */
  int tm_year;	/* Number of years 1900.  */
  int tm_wday;	/* Number of days since Sunday (0 through 6).  */
  int tm_yday;	/* Number of days since January 1 (0 through 365).  */
  int tm_isdst;	/* 1 - DST in effect, 0 - not, -1 - not known */
#ifndef __TARGET_SCL__
  int tm_gmtoff;	/* Offset east of UTC (GMT) in seconds.  */
  const char *tm_zone;	/* Abbreviation of timezone name.  */
#endif
};

/* Convert the broken-down time value into a string in a standard
   format:  "Sat Jul 12 14:47:12 1997\n"  */
extern char *asctime (const struct tm *__brokentime) __THROW;

/* Similar to asctime except that the time value is specified
   as a time_t calendar time value.  It is equivalent to asctime
   (localtime (time)).*/
extern char *ctime (const time_t *__timer) __THROW;

/* Return the number of seconds elapsed between time1 and time 0
   as a value of type double.  */
extern double difftime (time_t __time1, time_t __time0)
     __THROW __attribute__ ((__const__));

/* Convert a broken-down time structure to a calendar time
   representation.  */
extern time_t mktime (struct tm *__brokentime) __THROW;

/* Place characters into the 's' as controlled by the 'format'.
   'format' is specialised for printing components of 'brokentime'
   according to the locale currently specified for time conversion.  */
extern size_t strftime (char *__restrict __s, size_t __size,
			const char *__restrict __format,
       	      	        const struct tm *__restrict __brokentime) __THROW;

/* Convert the calendar time 'time' to broken-down time,
   expressed relative to the user's specified time zone. */
extern struct tm *localtime (const time_t *__timer) __THROW;

/* Similar to localtime() but the broken-down time is expressed
   as UTC (GMT) rather than the local time zone.  */
extern struct tm *gmtime (const time_t *__timer) __THROW;

__END_NAMESPACE_STD

# if defined __USE_POSIX || defined __USE_MISC
/* Convert the calendar time 'time' to broken-down time,
   expressed relative to the user's specified time zone (re-entrant). */
extern struct tm *localtime_r (const time_t *__restrict __timer,
			       struct tm *__restrict resultp) __THROW;

/* Similar to localtime() but the broken-down time is expressed
   as UTC (GMT) rather than the local time zone (re-entrant).  */
extern struct tm *gmtime_r (const time_t *__restrict __timer,
			    struct tm *__restrict resultp) __THROW;

/* Convert the broken-down time value into a string in a standard
   format:  "Sat Jul 12 14:47:12 1997\n" (re-entrant) */
extern char *asctime_r (const struct tm *__brokentime, char *__buffer) __THROW;

/* Similar to asctime except that the time value is specified
   as a time_t calendar time value. (re-entrant)  */
extern char *ctime_r (const time_t *__timer, char *__buffer) __THROW;

#endif

/* C99 Additions.  */

#ifdef __USE_POSIX199309

/* Identifier for system-wide realtime clock.  */
#define CLOCK_REALTIME 0

/* Monotonic system-wide clock.  */
#define CLOCK_MONOTONIC 1

/* High-resolution timer from the CPU.  */
#define CLOCK_PROCESS_CPUTIME_ID 2

/* Thread-specific CPU-time clock.  */
#define CLOCK_THREAD_CPUTIME_ID 3

/* Find the resolution of a specified clockid.  If 'res' is non NULL,
   then store the result into the structure pointed to by it.  */
extern int clock_getres (clockid_t __clk_id, struct timespec *__res) __THROW;

/* Retrieve the time of the specified clk_id.  */
extern int clock_gettime (clockid_t __clk_id, struct timespec *__tp) __THROW;

/* Set the time of the specified clk_id.  */
extern int clock_settime (clockid_t __clk_id, const struct timespec *__tp) __THROW;

#ifndef __TARGET_SCL__
/* Pause for a number of nanoseconds.
   This function is a cancellation point.  */
extern int nanosleep (const struct timespec *__req,
		      struct timespec *__rem);
#endif
#endif

/* System V compatibility.  */

# if defined __USE_SVID || defined __USE_XOPEN
/* The difference between UTC and the latest local standard time,
   in seconds west of UTC. The time is not adjusted for daylight
   saving and the sign is the reverse of tm_gmtoff.  */
extern int timezone;

/* Nonzero if daylight savings time rules apply.  */
extern int daylight;
#endif

/* Nonzero if YEAR is a leap year (every 4 years,
   except every 100th isn't, and every 400th is).  */
# define __isleap(year) \
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

# ifdef __USE_MISC
/* Miscellaneous functions many Unices inherited from the public domain
   localtime package.  These are included only for compatibility.  */

/* Like `mktime', but for TP represents Universal Time, not local time.  */
extern time_t timegm (struct tm *__tp) __THROW;

/* Another name for `mktime'.  */
extern time_t timelocal (struct tm *__tp) __THROW;

/* Return the number of days in YEAR.  */
extern int dysize (int __year) __THROW  __attribute__ ((__const__));
# endif


/* POSIX extensions.  */

#ifdef __USE_POSIX
/* Initialise the tzname variable with the TimeZone information from
   CMOS.  */
extern void tzset (void) __THROW;

/* tzname[0] is the name of the standard time zone.
   tzname[1] is the name for the time zone when daylight saving time
   is in use.  */
extern char *tzname[];
#endif

__END_DECLS

#endif /* __TIME_H */
#else
#undef __need_time_t
#undef __need_clock_t
#undef __need_timespec
#undef __need_clockid_t
#undef __need_timer_t
#endif /* ! __TIME_H */
