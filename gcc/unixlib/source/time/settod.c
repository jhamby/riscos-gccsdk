/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/settod.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: settod.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
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
