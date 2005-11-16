/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigismembe.c,v $
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

#undef sigismember

int
sigismember (const sigset_t * set, int sig)
{
  if (set == NULL || __SIG_INVALID_P (sig))
    return __set_errno (EINVAL);

  return (*set & sigmask (sig)) ? 1 : 0;
}
