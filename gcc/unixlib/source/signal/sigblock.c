/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigblock.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigblock.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
#endif

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
