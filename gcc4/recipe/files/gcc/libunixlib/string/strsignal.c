/* strsignal ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#ifndef __TARGET_SCL__
#  include <pthread.h>
#endif
#include <signal.h>
#include <stdio.h>
#include <string.h>

/* Return a string describing the meaning of the signal number SIGNUM.  */
char *
strsignal (int signum)
{
#ifndef __TARGET_SCL__
  static char ro_err[270];
#endif

  if (signum < 0 || signum >= NSIG)
    return "Unknown signal";

#ifndef __TARGET_SCL__
  if (signum == SIGOSERROR)
    {
      int bytesWritten;
      if ((bytesWritten = snprintf(ro_err, sizeof(ro_err), "%s (%s)",
				   sys_siglist[signum],
				    __pthread_running_thread->errbuf.errmess)) >= 0
	  && bytesWritten < sizeof(ro_err))
        return ro_err;
    }
#endif

  return (char *) sys_siglist[signum];
}
