/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/getuid,v $
 * $Date: 1997/10/08 12:48:13 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getuid,v 1.1 1997/10/08 12:48:13 unixlib Exp $";
#endif

#include <unistd.h>
#include <sys/unix.h>

__uid_t
getuid (void)
{
  return __u->uid;
}
