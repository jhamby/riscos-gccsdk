/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setgid.c,v $
 * $Date: 2000/07/15 14:52:28 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setgid.c,v 1.1.1.1 2000/07/15 14:52:28 nick Exp $";
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
