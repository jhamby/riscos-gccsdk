/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigblock,v $
 * $Date: 1997/10/09 20:00:29 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigblock,v 1.6 1997/10/09 20:00:29 unixlib Exp $";
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
