/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigpending,v $
 * $Date: 1997/10/09 20:00:30 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigpending,v 1.6 1997/10/09 20:00:30 unixlib Exp $";
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
