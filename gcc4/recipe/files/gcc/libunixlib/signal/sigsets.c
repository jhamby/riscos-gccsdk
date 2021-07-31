/* sigemptyset (), sigfillset (), sigmask ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <signal.h>

#include <internal/sigstate.h>

#undef sigemptyset
#undef sigfillset
#undef sigmask

int
sigemptyset (sigset_t *set)
{
  if (set == NULL)
    return __set_errno (EINVAL);

  *set = (sigset_t) 0;

  return 0;
}

int
sigfillset (sigset_t *set)
{
  if (set == NULL)
    return __set_errno (EINVAL);

  *set = ~(sigset_t) 0;

  return 0;
}

sigset_t
sigmask (int sig)
{
  if (__SIG_INVALID_P (sig))
    return __set_errno (EINVAL);

  return ((sigset_t) 1) << (sig - 1);
}
