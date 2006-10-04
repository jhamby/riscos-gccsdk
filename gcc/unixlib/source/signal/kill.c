/* kill ()
 * Written by Nick Burrett, 27 August 1996.
 * Copyright (c) 1996-2006 UnixLib Developers
 */

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <unixlib/unix.h>
#include <unixlib/sigstate.h>
#include <pthread.h>

int
kill (pid_t pid, int sig)
{
  PTHREAD_UNSAFE

  if (pid != __proc->pid)
    __unixlib_raise_signal (&__u->sigstate, sig);
  else
    __unixlib_raise_signal (0, sig);

  return 0;
}
