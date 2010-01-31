/* ftime ()
 * Written by Nick Burrett, 6 October 1996.
 * Copyright (c) 1996-2006 UnixLib Developers
 */

#include <stddef.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <errno.h>

/* Fill in timebuf with information about the current time.  */

int
ftime (struct timeb *timebuf)
{
  int temp;
  struct tm *tp;

  /* Save the errno because it could change in the forthcoming
     function calls.  */
  temp = errno;

  /* Reset errno so we can look for a possible error in time().  */
  errno = 0;
  if (time (&timebuf->time) == (time_t) - 1 || errno != 0)
    /* Time is not available.  */
    return -1;

  /* On RISC OS we don't support milliseconds.  */
  timebuf->millitm = 0;

  tp = localtime (&timebuf->time);
  if (tp == NULL)
    return -1;

  /* Specify if DST was used.  */
  timebuf->dstflag = tp->tm_isdst;

  /* Calculate the minutes west of GMT.  */
  timebuf->timezone = tp->tm_gmtoff / 60;

  /* Restore the original errno state. */
  errno = temp;

  return 0;
}
