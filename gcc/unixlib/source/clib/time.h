/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/h/time,v $
 * $Date: 2000/11/08 10:22:57 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* ANSI Standard 4.12: Date and Time <time.h>.  */

#ifndef __TIME_H
#define __TIME_H

#ifndef __STDDEF_H
#include <stddef.h>
#endif
#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* The number of clock ticks per second measured by the clock function.  */
#define CLOCKS_PER_SEC 100

/* Obsolete name for CLOCKS_PER_SEC.  */
#define CLK_TCK CLOCKS_PER_SEC

/* Data type used to represent clock ticks.  */
typedef __clock_t clock_t;

/* The data type used to represent calendar time. It represents
   the number of seconds elapsed since 00:00:00 on 1 January, 1970
   Universal Time Coordinated.  */
typedef __time_t time_t;

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
  char tm_zone[4];	/* abbreviation of timezone name */
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
   esxpressed relative to the user's specified time zone. */
extern struct tm *localtime (const time_t *__time);

/* Similar to localtime() but the broken-down time is expressed
   as UTC (GMT) rather than the local time zone.  */
extern struct tm *gmtime (const time_t *__time);

/* Convert a broken-down time structure to a calendar time
   representation.  */
extern time_t mktime (struct tm *__brokentime);

/* Place characters into the 's' as controlled by the 'format'.
   'format' is specialised for printing components of 'brokentime'
   according to the locale currently specified for time conversion.  */
extern size_t strftime (char *__s, size_t __size, const char *__format,
       	      	        const struct tm *__brokentime);


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
extern void __cvt_broken_time (const struct tm *__brokentime, char *__riscos_time);

/* Common function for localtime() and gmtime().  */
extern struct tm *__calendar_convert (int __swinum, const time_t *__tp);

/* Common function for ctime() and asctime().  */
extern char *__standard_time (const char *__riscos_time);
#endif

#ifdef __cplusplus
	}
#endif

#endif
