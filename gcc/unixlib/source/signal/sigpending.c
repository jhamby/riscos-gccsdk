/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigpending.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigpending.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
#endif

/* signal.c.sigpending.
   Written by Nick Burrett, 5 October 1996.  */

#include <errno.h>
#include <stddef.h>
#include <signal.h>
#include <sys/unix.h>

int
sigpending (sigset_t * set)
{
  if (set == NULL)
    return __set_errno (EINVAL);

  *set = __u->sigstate.pending;
  return 0;
}
