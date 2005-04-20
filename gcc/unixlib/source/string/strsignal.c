/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strsignal.c,v $
 * $Date: 2004/12/26 10:45:20 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strsignal.c,v 1.5 2004/12/26 10:45:20 peter Exp $";
#endif

#include <signal.h>
#include <stdio.h>
#include <string.h>


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
          sys_siglist[signum],
#if __UNIXLIB_FEATURE_PTHREADS
          __pthread_running_thread->errbuf.errmess
#else
          __ul_errbuf.errmess
#endif
          )) >= 0 && bytesWritten < sizeof(ro_err))
         return ro_err;
    }

  return (char *) sys_siglist[signum];
}
