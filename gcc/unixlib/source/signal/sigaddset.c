/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigaddset.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
