/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strsignal.c,v $
 * $Date: 2000/07/15 14:52:09 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strsignal.c,v 1.1.1.1 2000/07/15 14:52:09 nick Exp $";
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
