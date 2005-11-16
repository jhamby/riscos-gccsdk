/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stat/mkfifo.c,v $
 * $Date: 2002/11/18 15:42:33 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <errno.h>
#include <sys/stat.h>

int
mkfifo (const char *path, mode_t mode)
{
  path = path;
  mode = mode;

  return __set_errno (ENOSYS);
}
