/* sigaddset ()
 * Written by Nick Burrett, 6 October 1996.
 * Copyright (c) 1996-2006 UnixLib Developers
 */

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
