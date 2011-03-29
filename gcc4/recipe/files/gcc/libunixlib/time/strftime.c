/* UnixLib port by Nick Burrett, 13 July 1997.  */

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <swis.h>
#include <time.h>

#include <internal/os.h>

static char *add (const char *, char *, const char *);
static char *conv (int, const char *, char *, const char *);
static char *fmt (const char *, const struct tm *, char *,
       	    	  const char *, const char *);
static char *secs (const struct tm *, char *, const char *);
static char *territory_convert (const char *format, const char *timep,
				char *pt, const char *ptlim);
static char *territory_standard (int swinum, const char *timep, char *pt,
				const char *ptlim);

#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

size_t
strftime (char *s, const size_t maxsize,
	  const char *format, const struct tm *t)
{
  /* According to POSIX.1 every call to stftime implies a call to tzset.  */
  tzset ();

  const _kernel_oserror *err;
  char riscos_time[6];
  if ((err = __cvt_broken_time (t, riscos_time)) != NULL)
    {
      __ul_seterr (err, ENOSYS);
      return 0;
    }
  char *p = fmt ((format == NULL) ? "%c" : format, t, s, s + maxsize, riscos_time);
  if (p == s + maxsize)
    return 0;
  *p = '\0';
  return p - s;
}

static char *
fmt (const char *format, const struct tm *t, char *pt,
     const char *ptlim, const char *riscos_time)
{
  for (; *format; ++format)
    {
      if (*format == '%')
	{
	label:
	  switch (*++format)
	    {
	    case '\0':
	      --format;
	      break;
	    case 'A':
	      /* The full weekday name according to the current locale */
	      pt = territory_convert ("%WE", riscos_time, pt, ptlim);
	      continue;
	    case 'a':
	      /* The abbreviated weekday name according to the current locale */
	      pt = territory_convert ("%W3", riscos_time, pt, ptlim);
	      continue;
	    case 'B':
	      /* The full month name according to the current locale */
	      pt = territory_convert ("%MO", riscos_time, pt, ptlim);
	      continue;
	    case 'b':
	    case 'h': /* POSIX.2 extension */
	      /* The abbreviated month name according to the current locale */
	      pt = territory_convert ("%M3", riscos_time, pt, ptlim);
	      continue;
	    case 'C': /* POSIX.2 extension */
	      /* The century of the year.  */
	      pt = territory_convert ("%YR", riscos_time, pt, ptlim);
	      continue;
	    case 'c':
	      /* Preferred data and time representation for the current locale */
	      pt = territory_standard (Territory_ConvertStandardDateAndTime,
				       riscos_time, pt, ptlim);
	      continue;
	    case 'D': /* POSIX.2 extension */
	      /* The date using the format %m/%d/%y */
	      pt = fmt ("%m/%d/%y", t, pt, ptlim, riscos_time);
	      continue;
	    case 'd':
	      /* Day of the month as a decimal number */
	      pt = conv (t->tm_mday, "%02d", pt, ptlim);
	      continue;
	    case 'E':
	    case 'O':
	      /*
		 ** POSIX locale extensions, a la
		 ** Arnold Robbins' strftime version 3.0.
		 ** The sequences
		 **   %Ec %EC %Ex %Ey %EY
		 **   %Od %oe %OH %OI %Om %OM
		 **   %OS %Ou %OU %OV %Ow %OW %Oy
		 ** are supposed to provide alternate
		 ** representations.
		 ** (ado, 5/24/93)
	       */
	      goto label;
	    case 'e': /* POSIX.2 extension */
	      /* Day of the month, but padded with blank.  */
	      pt = conv (t->tm_mday, "%2d", pt, ptlim);
	      continue;
	    case 'H':
	      /* The hour as a decimal number, using 24 hour clock */
	      pt = conv (t->tm_hour, "%02d", pt, ptlim);
	      continue;
	    case 'I':
	      /* The hour as a decimal number, using 12 hour clock */
	      pt = conv ((t->tm_hour % 12) ?
			  (t->tm_hour % 12) : 12,
			  "%02d", pt, ptlim);
	      continue;
	    case 'j':
	      /* The day of the year as a decimal number */
	      pt = conv (t->tm_yday + 1, "%03d", pt, ptlim);
	      continue;
	    case 'k': /* GNU extension */
	      /* The hour as a decimal number, using 24 hour clock
		 but padded with blank */
	      pt = conv (t->tm_hour, "%2d", pt, ptlim);
	      continue;
	    case 'l':
	      /* The hour as a decimal number, using 12 hour clock
		 but padded with blank */
	      pt = conv ((t->tm_hour % 12) ?
			  (t->tm_hour % 12) : 12,
			  "%2d", pt, ptlim);
	      continue;
	    case 'M':
	      /* The minute as a decimal number.  */
	      pt = conv (t->tm_min, "%02d", pt, ptlim);
	      continue;
	    case 'm':
	      /* The month as a decimal number.  */
	      pt = conv (t->tm_mon + 1, "%02d", pt, ptlim);
	      continue;
	    case 'n':
	      /* A single newline character.  */
	      pt = add ("\n", pt, ptlim);
	      continue;
	    case 'p':
	      /* Either AM or PM according to the given time value.  */
	      pt = territory_convert ("%AM", riscos_time, pt, ptlim);
	      continue;
	    case 'R': /* GNU extension */
	      /* The hour and minute in decimal numbers using the format
		 %H:%M  */
	      pt = fmt ("%H:%M", t, pt, ptlim, riscos_time);
	      continue;
	    case 'r': /* POSIX.2 extension */
	      /* The complete time using the AM/PM format of the current
		 locale.  */
	      pt = fmt ("%I:%M:%S %p", t, pt, ptlim, riscos_time);
	      continue;
	    case 'S':
	      /* The second as a decimal number.  */
	      pt = conv (t->tm_sec, "%02d", pt, ptlim);
	      continue;
	    case 's': /* GNU extension */
	      /* The number of seconds since 1 Jan 1970 00:00:00 UTC.  */
	      pt = secs (t, pt, ptlim);
	      continue;
	    case 'T': /* POSIX.2 extension.  */
	      /* The time using decimal numbers using the format %H:%M:%S */
	      pt = fmt ("%H:%M:%S", t, pt, ptlim, riscos_time);
	      continue;
	    case 't': /* POSIX.2 extension.  */
	      /* A single tabulator character.  */
	      pt = add ("\t", pt, ptlim);
	      continue;
	    case 'U':
	      /* The week number of the current year as a decimal number.  */
	      pt = conv ((t->tm_yday + 7 - t->tm_wday) / 7,
			  "%02d", pt, ptlim);
	      continue;
	    case 'u': /* POSIX.2 extension.  */
	      /* The day of the week as a decimal number.  */
	      pt = conv ((t->tm_wday == 0) ? 7 : t->tm_wday,
			  "%d", pt, ptlim);
	      continue;
	    case 'V': /* POSIX.2 extension.  */
	      /* The ISO 8601:1988 week number as a decimal number.
		 ISO weeks start with Monday and end with Sunday.  */
	      {
		int i;

		i = (t->tm_yday + 10 - (t->tm_wday ?
					(t->tm_wday - 1) : 6)) / 7;
		if (i == 0)
		  {
		    /*
		       ** What day of the week does
		       ** January 1 fall on?
		     */
		    i = t->tm_wday -
		      (t->tm_yday - 1);
		    /*
		       ** Fri Jan 1: 53
		       ** Sun Jan 1: 52
		       ** Sat Jan 1: 53 if previous
		       **	     year a leap
		       **	     year, else 52
		     */
		    if (i == 5) /* friday */
		      i = 53;
		    else if (i == 0) /* sunday */
		      i = 52;
		    else
		      i = isleap (t->tm_year + 1900) ? 53 : 52;
#ifdef XPG4_1994_04_09
		    /*
		       ** As of 4/9/94, though,
		       ** XPG4 calls for 53
		       ** unconditionally.
		     */
		    i = 53;
#endif /* defined XPG4_1994_04_09 */
		  }
		pt = conv (i, "%02d", pt, ptlim);
	      }
	      continue;
	    case 'v':
	      /*
		 ** From Arnold Robbins' strftime version 3.0:
		 ** "date as dd-bbb-YYYY"
		 ** (ado, 5/24/93)
	       */
	      pt = fmt ("%e-%b-%Y", t, pt, ptlim, riscos_time);
	      continue;
	    case 'W':
	      /* The week number of the current year as a decimal
		 number.  */
	      pt = conv ((t->tm_yday + 7 -
			   (t->tm_wday ?
			    (t->tm_wday - 1) : 6)) / 7,
			  "%02d", pt, ptlim);
	      continue;
	    case 'w':
	      /* The day of the week as a decimal number.  */
	      pt = conv (t->tm_wday, "%d", pt, ptlim);
	      continue;
	    case 'X':
	      /* Preferred time representation for the current locale.  */
	      pt = territory_standard (Territory_ConvertStandardTime,
				       riscos_time, pt, ptlim);
	      continue;
	    case 'x':
	      /* Preferred date representation for the current locale.  */
	      pt = territory_standard (Territory_ConvertStandardDate,
				       riscos_time, pt, ptlim);
	      continue;
	    case 'y':
	      /* The year without a century as a decimal number.  */
	      pt = conv ((t->tm_year + 1900) % 100,
			  "%02d", pt, ptlim);
	      continue;
	    case 'Y':
	      /* The year as a decimal number, using the Gregorian calendar.  */
	      pt = conv (t->tm_year + 1900, "%04d", pt, ptlim);
	      continue;
	    case 'Z':
	      /* Time zone abbreviation.  */
	      if (t->tm_zone != NULL)
		pt = add (t->tm_zone, pt, ptlim);
	      else if (t->tm_isdst == 0 || t->tm_isdst == 1)
		{
		  pt = add (tzname[t->tm_isdst], pt, ptlim);
		}
	      else
		pt = add ("?", pt, ptlim);
	      continue;
#if 0
	    case '+':
	      pt = fmt (Locale->date_fmt, t, pt, ptlim, riscos_time);
	      continue;
#endif
	    case '%':
	      /*
	       * X311J/88-090 (4.12.3.5): if conversion char is
	       * undefined, behavior is undefined.  Print out the
	       * character itself as printf(3) also does.
	       */
	    default:
	      break;
	    }
	}
      if (pt == ptlim)
	break;
      *pt++ = *format;
    }
  return pt;
}

static char *
conv (const int n, const char *format, char *pt, const char *ptlim)
{
  char buf[32];

  sprintf (buf, format, n);
  return add (buf, pt, ptlim);
}

static char *
secs (const struct tm *t, char *pt, const char *ptlim)
{
  char buf[32];
  time_t s;
  struct tm tmp;

  /* Make a copy, mktime(3) modifies the tm struct. */
  tmp = *t;
  s = mktime (&tmp);
  sprintf (buf, "%d", (int)s);
  return add (buf, pt, ptlim);
}

static char *
add (const char *str, char *pt, const char *ptlim)
{
  while (pt < ptlim && (*pt = *str++) != '\0')
    ++pt;
  return pt;
}

static char *
territory_convert (const char *format, const char *timep,
		   char *pt, const char *ptlim)
{
  int regs[10];
  char buffer[64];

  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)timep;
  regs[2] = (int)buffer;
  regs[3] = sizeof (buffer) - 1;
  regs[4] = (int)format;
  __os_swi (Territory_ConvertDateAndTime, regs);
  return add (buffer, pt, ptlim);
}

static char *
territory_standard (int swinum, const char *timep, char *pt, const char *ptlim)
{
  int regs[10];
  char buffer[64];

  regs[0] = __locale_territory[LC_TIME];
  regs[1] = (int)timep;
  regs[2] = (int)buffer;
  regs[3] = sizeof (buffer) - 1;
  __os_swi (swinum, regs);
  return add (buffer, pt, ptlim);
}
