/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigpause,v $
 * $Date: 1997/10/09 20:00:29 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigpause,v 1.6 1997/10/09 20:00:29 unixlib Exp $";
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
