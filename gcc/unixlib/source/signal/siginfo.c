/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/siginfo.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: siginfo.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
#endif

/* Written by Nick Burrett, 27 August 1996.  */

#include <sys/os.h>
#include <signal.h>
#include <unixlib/sigstate.h>

/* We should print something relatively informative here.
   But I can't think of anything to print.  */

void
__unixlib_siginfo_handler (int signo)
{
  signo = signo;
  /* Use os_print to avoid pulling in stdio unnecessarily.  */
  os_print ("got a SIGINFO\r\n");
}
