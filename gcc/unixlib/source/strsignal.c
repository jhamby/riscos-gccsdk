/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strsignal.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strsignal.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
#endif

#include <signal.h>
#include <stdio.h>
#include <string.h>


/* Return a string describing the meaning of the signal number SIGNUM.  */
char *
strsignal (int signum)
{
  if (signum < 0 || signum >= NSIG)
    {
      static char unknown_signal[] = "Unknown signal 00000000000";

      sprintf (unknown_signal, "Unknown signal %d", signum);
      return unknown_signal;
    }

  return (char *) sys_siglist[signum];
}
