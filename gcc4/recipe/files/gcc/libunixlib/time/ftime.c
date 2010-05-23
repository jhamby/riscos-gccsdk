/* ftime ()
 * Written by Nick Burrett, 6 October 1996.
 * Copyright (c) 1996-2010 UnixLib Developers
 */

#include <errno.h>
#include <stddef.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <time.h>

/* Fill in timebuf with information about the current time.  */
int
ftime (struct timeb *timebuf)
{
  /* Save the errno because it could change in the forthcoming
     function calls.  */
  const int temp = errno;

  /* Reset errno so we can look for a possible error in time().  */
  errno = 0;
  if (time (&timebuf->time) == (time_t) - 1 || errno != 0)
    /* Time is not available.  */
    return -1;

  /* On RISC OS we don't support milliseconds.  */
  timebuf->millitm = 0;

  const struct tm *tp = localtime (&timebuf->time);
  if (tp == NULL)
    return -1;

  /* Specify if DST was used.  */
  timebuf->dstflag = tp->tm_isdst;

  /* Calculate the minutes west of GMT.  */
#ifndef __TARGET_SCL__
  timebuf->timezone = tp->tm_gmtoff / 60;
#else
  timebuf->timezone = 0;
#endif

  /* Restore the original errno state. */
  errno = temp;

  return 0;
}
