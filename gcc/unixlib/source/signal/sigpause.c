/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigpause.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigpause.c,v 1.2 2001/01/29 15:10:21 admin Exp $";
#endif

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
