/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/proc.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: proc.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <unistd.h>
#include <unixlib/unix.h>

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
