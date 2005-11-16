/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/seteuid.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

int
seteuid (__uid_t uid)
{
  PTHREAD_UNSAFE

  if (uid == __proc->euid)
    return 0;
  if (uid == __proc->uid)
    {
      __proc->euid = uid;
      return 0;
    }
  return -1;
}
