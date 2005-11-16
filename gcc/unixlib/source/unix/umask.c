/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/umask.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <unixlib/types.h>
#include <sys/stat.h>
#include <unixlib/unix.h>
#include <pthread.h>

__mode_t
umask (__mode_t cmode)
{
  __mode_t old_umask;

  PTHREAD_UNSAFE

  old_umask = __proc->umask;

  __proc->umask = cmode & 0777;
  return old_umask;
}
