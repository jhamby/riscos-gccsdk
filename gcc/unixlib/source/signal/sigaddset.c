/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigaddset.c,v $
 * $Date: 2000/07/15 14:52:29 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigaddset.c,v 1.1.1.1 2000/07/15 14:52:29 nick Exp $";
#endif

/* Written by Nick Burrett, 6 October 1996.  */

#include <errno.h>
#include <signal.h>
#include <unixlib/sigstate.h>

#undef sigaddset

int
sigaddset (__sigset_t * set, int sig)
{
  if (__SIG_INVALID_P (sig))
    return __set_errno (EINVAL);

  *set |= sigmask (sig);
  return 0;
}
