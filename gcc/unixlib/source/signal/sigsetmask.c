/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigsetmask.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigsetmask.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
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
