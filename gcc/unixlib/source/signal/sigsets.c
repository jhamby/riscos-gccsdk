/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigsets.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigsets.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
#endif

#include <errno.h>
#include <signal.h>
#include <unixlib/sigstate.h>

#undef sigemptyset
#undef sigfillset
#undef sigmask

int sigemptyset (sigset_t *set)
{
  if (set == NULL)
    return __set_errno (EINVAL);

  *set = (sigset_t) 0;

  return 0;
}

int sigfillset (sigset_t *set)
{
  if (set == NULL)
    return __set_errno (EINVAL);

  *set = ~(sigset_t) 0;

  return 0;
}

sigset_t sigmask (int sig)
{
  if (__SIG_INVALID_P (sig))
    return __set_errno (EINVAL);

  return (((sigset_t) 1) << (sig - 1));
}
