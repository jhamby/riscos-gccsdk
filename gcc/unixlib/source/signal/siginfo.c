/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/siginfo.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Written by Nick Burrett, 27 August 1996.  */

#include <unixlib/os.h>
#include <signal.h>
#include <unixlib/sigstate.h>

/* We should print something relatively informative here.
   But I can't think of anything to print.  */

void
__unixlib_siginfo_handler (int signo)
{
  signo = signo;
  /* Use __os_print to avoid pulling in stdio unnecessarily.  */
  __os_print ("got a SIGINFO\r\n");
}
