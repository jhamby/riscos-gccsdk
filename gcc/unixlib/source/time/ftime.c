/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/ftime.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ftime.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
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
