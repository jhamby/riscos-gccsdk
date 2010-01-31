/* kill ()
 * Written by Nick Burrett, 27 August 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#include <pthread.h>
#include <internal/unix.h>
#include <internal/sigstate.h>

int
kill (pid_t pid, int sig)
{
  struct ul_global *gbl = &__ul_global;
  struct __sul_process *sulproc = gbl->sulproc;

  PTHREAD_UNSAFE

  if (pid != sulproc->pid)
    __unixlib_raise_signal (&__u->sigstate, sig);
  else
    __unixlib_raise_signal (0, sig);

  return 0;
}
