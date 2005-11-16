/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigpause.c,v $
 * $Date: 2003/04/28 21:04:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

/* signal.c.sigpause: BSD signal function.

   Written by Nick Burrett, 5 October 1996.  */

#include <signal.h>
#include <pthread.h>

/* Set the mask of blocked signals to MASK,
   wait for a signal to arrive, and then restore the mask.  */
int
sigpause (int mask)
{
  sigset_t set;

  PTHREAD_SAFE_CANCELLATION

  set = (sigset_t) mask;

  return sigsuspend (&set);
}
