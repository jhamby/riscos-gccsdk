/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/abort,v $
 * $Date: 1997/10/09 19:59:36 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: abort,v 1.5 1997/10/09 19:59:36 unixlib Exp $";
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
