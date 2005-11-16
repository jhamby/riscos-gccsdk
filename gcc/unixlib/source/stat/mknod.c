/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stat/mknod.c,v $
 * $Date: 2002/11/18 15:42:33 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <errno.h>
#include <sys/stat.h>

int
mknod (const char *path, mode_t mode, dev_t dev)
{
  path = path;
  mode = mode;
  dev = dev;

  return __set_errno (ENOSYS);
}
