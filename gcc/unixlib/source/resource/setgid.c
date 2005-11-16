/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setgid.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

/* Set the real and effective group ID of the process to gid.  */

int
setgid (__gid_t gid)
{
  PTHREAD_UNSAFE
  
  if (gid == __proc->gid)
    return 0;
  if (gid == __proc->egid)
    {
      __proc->gid = gid;
      return 0;
    }
  return -1;
}
