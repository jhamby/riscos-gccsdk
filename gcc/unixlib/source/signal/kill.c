/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/kill,v $
 * $Date: 1997/10/09 20:00:28 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: kill,v 1.6 1997/10/09 20:00:28 unixlib Exp $";
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
