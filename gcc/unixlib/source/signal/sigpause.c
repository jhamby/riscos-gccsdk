/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigpause.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigpause.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
#endif

/* signal.c.sigpause: BSD signal function.

   Written by Nick Burrett, 5 October 1996.  */

#include <signal.h>

/* Set the mask of blocked signals to MASK,
   wait for a signal to arrive, and then restore the mask.  */
int
sigpause (int mask)
{
  sigset_t set;

  set = (sigset_t) mask;

  return sigsuspend (&set);
}
