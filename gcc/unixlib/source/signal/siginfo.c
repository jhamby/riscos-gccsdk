/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/siginfo,v $
 * $Date: 2000/06/03 14:46:10 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: siginfo,v 1.6 2000/06/03 14:46:10 admin Exp $";
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
