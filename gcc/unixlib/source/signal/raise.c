/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/raise,v $
 * $Date: 1997/10/09 20:00:28 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: raise,v 1.6 1997/10/09 20:00:28 unixlib Exp $";
#endif

/* Written by Nick Burrett, 26 August 1996.  */

#include <errno.h>
#include <signal.h>
#include <sys/unix.h>
#include <unixlib/sigstate.h>

int
raise (int sig)
{
  if (__SIG_INVALID_P (sig))
    return __set_errno (EINVAL);

  __unixlib_raise_signal (0, sig);

  return 0;
}
