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

#include <errno.h>
#include <unistd.h>
#include <unixlib/unix.h>

int
setegid (gid_t gid)
{
  if (gid == __u->egid)
    return 0;
  if (gid == __u->gid)
    {
      __u->egid = gid;
      return 0;
    }

  return __set_errno (EPERM);
}
