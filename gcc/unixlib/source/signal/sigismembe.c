/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigismembe.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigismembe.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
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
