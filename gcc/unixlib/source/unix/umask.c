/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/umask,v $
 * $Date: 1997/10/09 20:00:53 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: umask,v 1.6 1997/10/09 20:00:53 unixlib Exp $";
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
