/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/proc.c,v $
 * $Date: 2000/07/15 14:52:44 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: proc.c,v 1.1.1.1 2000/07/15 14:52:44 nick Exp $";
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
