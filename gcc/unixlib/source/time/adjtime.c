/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/adjtime.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
