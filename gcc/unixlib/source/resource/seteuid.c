/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/seteuid.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: seteuid.c,v 1.4 2003/01/21 17:54:22 admin Exp $";
#endif

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
