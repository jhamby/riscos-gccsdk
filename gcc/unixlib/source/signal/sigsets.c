/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigsets.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
