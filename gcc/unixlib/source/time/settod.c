/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/settod,v $
 * $Date: 1997/10/09 20:00:46 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: settod,v 1.5 1997/10/09 20:00:46 unixlib Exp $";
#endif

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
