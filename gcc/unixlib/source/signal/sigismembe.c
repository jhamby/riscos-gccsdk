/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigismembe,v $
 * $Date: 1997/10/09 20:00:29 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigismembe,v 1.7 1997/10/09 20:00:29 unixlib Exp $";
#endif

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
