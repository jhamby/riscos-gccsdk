/* __unixlib_siginfo_handler ()
 * Written by Nick Burrett, 27 August 1996.
 * Copyright (c) 1996-2006 UnixLib Developers
 */

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
