/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/abort.c,v $
 * $Date: 2003/01/05 12:40:04 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: abort.c,v 1.3 2003/01/05 12:40:04 admin Exp $";
#endif

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
