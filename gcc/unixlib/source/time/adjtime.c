/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/adjtime.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: adjtime.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
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
