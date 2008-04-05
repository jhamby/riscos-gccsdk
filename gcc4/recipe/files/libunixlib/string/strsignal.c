/* strsignal ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include <pthread.h>

/* Return a string describing the meaning of the signal number SIGNUM.  */
char *
strsignal (int signum)
{
  static char ro_err[270];

  if (signum < 0 || signum >= NSIG)
    return "Unknown signal";

  if (signum == SIGOSERROR)
    {
      int bytesWritten;

      if ((bytesWritten = snprintf(ro_err, sizeof(ro_err), "%s (%s)",
          sys_siglist[signum], __pthread_running_thread->errbuf.errmess)) >= 0
	  && bytesWritten < sizeof(ro_err))
        return ro_err;
    }

  return (char *) sys_siglist[signum];
}
