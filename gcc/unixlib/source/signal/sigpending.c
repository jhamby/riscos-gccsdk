/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigpending.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigpending.c,v 1.3 2002/02/14 15:56:36 admin Exp $";
#endif

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
