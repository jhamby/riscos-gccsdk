/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/settod.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <errno.h>
#include <sys/time.h>

/* Set the current time of day and timezone information.  */
int
settimeofday (const struct timeval *tv, const struct timezone *tz)
{
  tv = tv;
  tz = tz;
  (void) __set_errno (EPERM);
  return -1;
}
