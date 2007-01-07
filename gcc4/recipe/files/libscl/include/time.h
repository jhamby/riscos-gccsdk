/* time.h standard header for the RISC OS SharedCLibrary.
   Copyright (c) 1997-2005 Nick Burrett
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef __TIME_H
#define __TIME_H

#ifndef __STDDEF_H
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __CLK_TCK
/* Obsolete name for CLOCKS_PER_SEC.  */
#define CLK_TCK __CLK_TCK
/* Number of clock ticks per second measured by the clock function.  */
#define CLOCKS_PER_SEC __CLK_TCK
#else
#define CLK_TCK 100
#define CLOCKS_PER_SEC 100
#endif

/* Type of the value returned by the clock() function. Represents
   the number of seconds elapsed since 00:00:00 on January 1, 1970.   */
typedef unsigned int clock_t;

/* Returns the elapsed processor time since program execution. */
extern clock_t clock (void);

/* Calendar time.  */
typedef unsigned int time_t;

struct tm
{
  /* Number of seconds after the minute (0 through 59).  */
  int tm_sec;
  /* Number of minutes after the hour (0 through 59).  */
  int tm_min;
  /* Number of hours past midnight (0 through 23).  */
  int tm_hour;
  /* Day of the month (1 through 31).  */
  int tm_mday;
  /* Number of months since January (0 through 11).  */
  int tm_mon;
  /* Number of years since 1900.  */
  int tm_year;
  /* Number of days since Sunday (0 through 6).  */
  int tm_wday;
  /* Number of days since January 1 (0 through 365).  */
  int tm_yday;
  /* This flag indicates whether Daylight Saving Time is
     (or was, or will be) in effect at the time described.
     The value is positive is DST is in effect, zero if it is not,
     and negative if the information is not available.  */
  int tm_isdst;
};

/* Return the number of seconds elapsed between time1 and time0
   as a value of type double.  */
extern double difftime (time_t __time1, time_t __time0);

/* Convert a broken-down time structure to a calendar time
   representation.  */
extern time_t mktime (struct tm *__brokentime);

/* Return the current time as a value of type time_t.
   If 'result' is non null, then the time will also be stored
   in here.  */
extern time_t time (time_t *__result);

/* Convert the broken-down time value that 'brokentime'
   points to into a string in a standard format:

     "Sat Mar 22 12:16:34 1997\n".  */
extern char *asctime (const struct tm *__brokentime);

/* Similar to asctime() except that the time value is
   specified as a time_t calendar time value.  It is
   equivalent to asctime (localtime (time)).  */
extern char *ctime (const time_t *__time);

/* Convert the calendar time pointed to by 'time' to broken-down
   time representation, expressed relative to the user's
   specified time zone.  */
extern struct tm *localtime (const time_t *__time);

/* Similar to localtime() except that the broken-down time
   is expressed as GMT, rather than relative to the local time zone.  */
extern struct tm *gmtime (const time_t *__time);

/* Similar to the sprintf function but the conversion specifications
   that can appear in the format template 'template' are specialized
   for printing components of the date and time 'brokentime' according
   to the locale currently specified for time conversion.  */
extern size_t strftime (char *__s, size_t __size,
			const char *__templ,
			const struct tm *__brokentime);

#ifdef __cplusplus
}
#endif

#endif
