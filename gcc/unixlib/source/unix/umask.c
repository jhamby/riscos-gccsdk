/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/umask.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: umask.c,v 1.4 2003/04/05 09:33:57 alex Exp $";
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

  old_umask = __proc->umask;

  __proc->umask = cmode & 0777;
  return old_umask;
}
