/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/init.c,v $
 * $Date: 2000/07/15 14:52:29 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: init.c,v 1.1.1.1 2000/07/15 14:52:29 nick Exp $";
#endif

/* Initialise the UnixLib stack handlers.
   Written by Nick Burrett, 26 August 1996.  */

#include <stdlib.h>
#include <signal.h>
#include <unixlib/sigstate.h>
#include <sys/unix.h>
#include <sys/syslib.h>
#include <unistd.h>
#include <sys/os.h>

void
__unixlib_signal_initialise (struct proc *p)
{
#ifdef DEBUG
  os_print ("__unixlib_signal_initialise\r\n");
#endif

  /* Initialise signal handlers.  */
  __unixlib_default_sigaction (&p->sigstate);

  p->sigstate.currently_handling = 0;

  p->sigstate.signalstack.ss_sp = 0;
  p->sigstate.signalstack.ss_size = 0;
  p->sigstate.signalstack.ss_flags = SA_DISABLE;
}
