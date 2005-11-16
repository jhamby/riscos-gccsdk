/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setsid.c,v $
 * $Date: 2003/04/21 13:20:39 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#include <unistd.h>
#include <unixlib/unix.h>
#include <errno.h>

/* Creates a new session if the calling process is not a process group leader */

__pid_t
setsid (void)
{
  return __set_errno (ENOSYS);
}
