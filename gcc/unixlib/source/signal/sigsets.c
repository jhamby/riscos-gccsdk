/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigsets,v $
 * $Date: 1997/10/09 20:00:30 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigsets,v 1.7 1997/10/09 20:00:30 unixlib Exp $";
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
