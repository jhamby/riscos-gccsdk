/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigpending.c,v $
 * $Date: 2003/04/28 21:04:36 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

/* signal.c.sigpending.
   Written by Nick Burrett, 5 October 1996.  */

#include <errno.h>
#include <stddef.h>
#include <signal.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
sigpending (sigset_t * set)
{
  PTHREAD_UNSAFE

  if (set == NULL)
    return __set_errno (EINVAL);

  *set = __u->sigstate.pending;
  return 0;
}
