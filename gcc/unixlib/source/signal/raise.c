/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/raise.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: raise.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

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
