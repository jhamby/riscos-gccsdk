/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/kill.c,v $
 * $Date: 2000/07/15 14:52:29 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: kill.c,v 1.1.1.1 2000/07/15 14:52:29 nick Exp $";
#endif

/* Written by Nick Burrett, 27 August 1996.  */

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/unix.h>
#include <unixlib/sigstate.h>

int
kill (pid_t pid, int sig)
{
  if (pid != __u->pid)
    __unixlib_raise_signal (&__u->sigstate, sig);
  else
    __unixlib_raise_signal (0, sig);

  return 0;
}
