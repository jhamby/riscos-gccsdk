/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigsetmask,v $
 * $Date: 1997/10/09 20:00:30 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigsetmask,v 1.6 1997/10/09 20:00:30 unixlib Exp $";
#endif

/* sigsetmask.c: BSD signal blocking function.
   Written by Nick Burrett, 5 October 1996.  */

#include <signal.h>

/* Set the mask of blocked signals to MASK, returning the old mask.  */
int
sigsetmask (int mask)
{
  sigset_t set, oset;

  set = (sigset_t) mask;

  if (sigprocmask (SIG_SETMASK, &set, &oset) < 0)
    return -1;

  return (int) oset;
}
