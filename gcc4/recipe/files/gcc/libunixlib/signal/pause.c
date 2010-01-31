/* pause ()
 * Written by Nick Burrett, 5 October 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <signal.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

/* Suspend the process until a signal arrives.
   This always returns -1 and sets errno to EINTR.  */

int
pause (void)
{
  PTHREAD_SAFE_CANCELLATION

  /* errno is set to EINTR by sigsuspend() which is called
     through sigpause().  */
  return sigpause (sigblock (0));
}
