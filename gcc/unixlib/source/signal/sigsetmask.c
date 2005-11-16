/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigsetmask.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
