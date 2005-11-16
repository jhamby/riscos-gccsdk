/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdlib/abort.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void
abort (void)
{
  sigset_t sigs;

  PTHREAD_UNSAFE

  sigs = sigmask (SIGABRT);
  sigprocmask (SIG_UNBLOCK, &sigs, NULL);
  raise (SIGABRT);
  /* If the signal fails, exit.  */
  _exit (127);
}
