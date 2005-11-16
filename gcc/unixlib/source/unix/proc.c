/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/proc.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
