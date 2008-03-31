/* sigpending ()
 * Written by Nick Burrett, 5 October 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <errno.h>
#include <stddef.h>
#include <signal.h>

#include <internal/unix.h>
#include <pthread.h>

int
sigpending (sigset_t * set)
{
  PTHREAD_UNSAFE

  if (set == NULL)
    return __set_errno (EINVAL);

  *set = __u->sigstate.pending;
  return 0;
}
