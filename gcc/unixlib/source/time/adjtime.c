/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/adjtime,v $
 * $Date: 1997/10/09 20:00:44 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: adjtime,v 1.5 1997/10/09 20:00:44 unixlib Exp $";
#endif

#include <sys/time.h>

/* This function speeds up or slows down the system clock in order to
   make gradual adjustments in the current time.  This ensures that
   the time reported by the system clock is always monotonically
   increasing, which might not happen if you simply set the current
   time.  Irrelevant on RISC OS.  */

int
adjtime (const struct timeval *delta, struct timeval *olddelta)
{
  olddelta = olddelta;
  delta = delta;
  return -1;
}
