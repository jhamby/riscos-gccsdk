/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strsignal,v $
 * $Date: 1997/10/09 19:59:42 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strsignal,v 1.6 1997/10/09 19:59:42 unixlib Exp $";
#endif

#include <signal.h>
#include <stdio.h>
#include <string.h>


/* Return a string describing the meaning of the signal number SIGNUM.  */
const char *
strsignal (int signum)
{
  if (signum < 0 || signum > NSIG)
    {
      static char unknown_signal[] = "Unknown signal 00000000000";

      sprintf (unknown_signal, "Unknown signal %d", signum);
      return unknown_signal;
    }

  return sys_siglist[signum];
}
