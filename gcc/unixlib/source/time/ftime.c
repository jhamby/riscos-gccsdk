/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/ftime,v $
 * $Date: 1997/10/09 20:00:45 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ftime,v 1.6 1997/10/09 20:00:45 unixlib Exp $";
#endif

/* time.c.ftime.
   Function implementation for <sys/timeb.h>.

   Written by Nick Burrett, 6 October 1996.  */

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
