/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/geteuid,v $
 * $Date: 1997/10/08 12:48:12 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: geteuid,v 1.1 1997/10/08 12:48:12 unixlib Exp $";
#endif

#include <unistd.h>
#include <sys/unix.h>

__uid_t
geteuid (void)
{
  return __u->euid;
}
