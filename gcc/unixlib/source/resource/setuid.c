/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/setuid,v $
 * $Date: 1997/10/08 12:48:13 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setuid,v 1.1 1997/10/08 12:48:13 unixlib Exp $";
#endif

#include <unistd.h>
#include <sys/unix.h>

int
setuid (__uid_t uid)
{
  if (uid == __u->uid)
    return 0;
  if (uid == __u->euid)
    {
      __u->uid = uid;
      return 0;
    }
  return -1;
}
