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

int
setuid (__uid_t uid)
{
  PTHREAD_UNSAFE
  
  if (uid == __u->uid)
    return 0;
  if (uid == __u->euid)
    {
      __u->uid = uid;
      return 0;
    }
  return -1;
}
