/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigpending.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigpending.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

/* signal.c.sigpending.
   Written by Nick Burrett, 5 October 1996.  */

#include <errno.h>
#include <stddef.h>
#include <signal.h>
#include <unixlib/unix.h>

int
sigpending (sigset_t * set)
{
  if (set == NULL)
    return __set_errno (EINVAL);

  *set = __u->sigstate.pending;
  return 0;
}
