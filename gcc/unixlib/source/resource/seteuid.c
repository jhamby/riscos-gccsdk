/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/seteuid,v $
 * $Date: 1997/10/08 12:48:13 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: seteuid,v 1.1 1997/10/08 12:48:13 unixlib Exp $";
#endif

#include <unistd.h>
#include <sys/unix.h>

int
seteuid (__uid_t uid)
{
  if (uid == __u->euid)
    return 0;
  if (uid == __u->uid)
    {
      __u->euid = uid;
      return 0;
    }
  return -1;
}
