/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigaddset,v $
 * $Date: 1997/10/09 20:00:28 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigaddset,v 1.7 1997/10/09 20:00:28 unixlib Exp $";
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
