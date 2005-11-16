/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/geteuid.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <unistd.h>
#include <unixlib/unix.h>

__uid_t
geteuid (void)
{
  return __proc->euid;
}
