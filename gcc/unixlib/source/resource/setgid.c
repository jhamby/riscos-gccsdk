/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/setgid,v $
 * $Date: 1997/10/08 12:48:13 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setgid,v 1.1 1997/10/08 12:48:13 unixlib Exp $";
#endif

#include <unistd.h>
#include <sys/unix.h>

/* Set the real and effective group ID of the process to gid.  */

int
setgid (__gid_t gid)
{
  if (gid == __u->gid)
    return 0;
  if (gid == __u->egid)
    {
      __u->gid = gid;
      return 0;
    }
  return -1;
}
