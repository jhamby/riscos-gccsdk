/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/proc.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: proc.c,v 1.5 2003/04/05 09:33:57 alex Exp $";
#endif

#include <errno.h>
#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
setegid (gid_t gid)
{
  PTHREAD_UNSAFE

  if (gid == __proc->egid)
    return 0;
  if (gid == __proc->gid)
    {
      __proc->egid = gid;
      return 0;
    }

  return __set_errno (EPERM);
}
