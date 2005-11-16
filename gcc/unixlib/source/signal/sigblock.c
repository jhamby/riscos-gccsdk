/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigblock.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* signal.c.sigblock: BSD signal functions.

   Written by Nick Burrett, 5 October 1996.  */

#include <signal.h>

/* Block signals in MASK, returning the old mask.  */
int
sigblock (int mask)
{
  sigset_t set, oset;

  set = (sigset_t) mask;

  if (sigprocmask (SIG_BLOCK, &set, &oset) < 0)
    return -1;

  return (int) oset;
}
