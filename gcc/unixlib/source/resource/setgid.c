/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setgid.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setgid.c,v 1.4 2003/01/21 17:54:22 admin Exp $";
#endif

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
