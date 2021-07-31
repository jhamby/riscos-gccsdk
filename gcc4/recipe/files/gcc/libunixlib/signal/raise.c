/* raise ()
 * Written by Nick Burrett, 26 August 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <errno.h>
#include <signal.h>

#include <internal/unix.h>
#include <internal/sigstate.h>

int
raise (int sig)
{
  if (__SIG_INVALID_P (sig))
    return __set_errno (EINVAL);

  __unixlib_raise_signal (0, sig);

  return 0;
}
