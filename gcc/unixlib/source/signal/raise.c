/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/raise.c,v $
 * $Date: 2000/07/15 14:52:29 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: raise.c,v 1.1.1.1 2000/07/15 14:52:29 nick Exp $";
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
