/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/getpid,v $
 * $Date: 1997/10/08 12:48:12 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpid,v 1.1 1997/10/08 12:48:12 unixlib Exp $";
#endif

#include <unistd.h>
#include <sys/unix.h>

__pid_t
getpid (void)
{
  return __u->pid;
}
