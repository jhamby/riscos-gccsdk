/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

#include <unistd.h>
#include <unixlib/unix.h>
#include <pthread.h>

/* Set the real and effective group ID of the process to gid.  */

int
setgid (__gid_t gid)
{
  PTHREAD_UNSAFE
  
  if (gid == __u->gid)
    return 0;
  if (gid == __u->egid)
    {
      __u->gid = gid;
      return 0;
    }
  return -1;
}
