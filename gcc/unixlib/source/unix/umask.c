/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/umask.c,v $
 * $Date: 2000/07/15 14:52:45 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: umask.c,v 1.1.1.1 2000/07/15 14:52:45 nick Exp $";
#endif

#include <unixlib/types.h>
#include <sys/stat.h>
#include <sys/unix.h>

__mode_t
umask (__mode_t cmode)
{
  __mode_t old_umask = __u->umask;

  __u->umask = cmode & 0777;
  return old_umask;
}
