/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/getegid,v $
 * $Date: 1997/10/08 12:48:11 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getegid,v 1.1 1997/10/08 12:48:11 unixlib Exp $";
#endif

#include <unistd.h>
#include <sys/unix.h>

__gid_t
getegid (void)
{
  return __u->egid;
}
