/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/umask.c,v $
 * $Date: 2002/02/14 15:56:39 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: umask.c,v 1.3 2002/02/14 15:56:39 admin Exp $";
#endif

#include <unixlib/types.h>
#include <sys/stat.h>
#include <unixlib/unix.h>
#include <pthread.h>

__mode_t
umask (__mode_t cmode)
{
  __mode_t old_umask;

  PTHREAD_UNSAFE

  old_umask = __u->umask;

  __u->umask = cmode & 0777;
  return old_umask;
}
