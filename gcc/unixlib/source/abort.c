/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/abort.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: abort.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
#endif

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void
abort (void)
{
  sigset_t sigs;

  sigs = sigmask (SIGABRT);
  sigprocmask (SIG_UNBLOCK, &sigs, NULL);
  raise (SIGABRT);
  /* If the signal fails, exit.  */
  _exit (127);
}
