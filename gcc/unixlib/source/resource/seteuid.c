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
seteuid (__uid_t uid)
{
  PTHREAD_UNSAFE

  if (uid == __u->euid)
    return 0;
  if (uid == __u->uid)
    {
      __u->euid = uid;
      return 0;
    }
  return -1;
}
