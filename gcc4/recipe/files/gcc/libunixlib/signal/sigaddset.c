/* sigaddset ()
 * Written by Nick Burrett, 6 October 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <errno.h>
#include <signal.h>

#include <internal/sigstate.h>

#undef sigaddset

int
sigaddset (__sigset_t * set, int sig)
{
  if (__SIG_INVALID_P (sig))
    return __set_errno (EINVAL);

  *set |= sigmask (sig);
  return 0;
}
