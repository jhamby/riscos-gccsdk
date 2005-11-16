/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/raise.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Written by Nick Burrett, 26 August 1996.  */

#include <errno.h>
#include <signal.h>
#include <unixlib/unix.h>
#include <unixlib/sigstate.h>

int
raise (int sig)
{
  if (__SIG_INVALID_P (sig))
    return __set_errno (EINVAL);

  __unixlib_raise_signal (0, sig);

  return 0;
}
