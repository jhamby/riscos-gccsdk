/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/proc.c,v $
 * $Date: 2002/09/24 21:02:38 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: proc.c,v 1.4 2002/09/24 21:02:38 admin Exp $";
#endif

#include <errno.h>
#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
setegid (gid_t gid)
{
  PTHREAD_UNSAFE

  if (gid == __u->egid)
    return 0;
  if (gid == __u->gid)
    {
      __u->egid = gid;
      return 0;
    }

  return __set_errno (EPERM);
}
