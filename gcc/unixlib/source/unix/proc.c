/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/proc,v $
 * $Date: 1997/10/08 12:48:15 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: proc,v 1.6 1997/10/08 12:48:15 unixlib Exp $";
#endif

#include <unistd.h>
#include <sys/unix.h>

int
setegid (__gid_t gid)
{
  if (gid == __u->egid)
    return 0;
  if (gid == __u->gid)
    {
      __u->egid = gid;
      return 0;
    }
  return -1;
}
