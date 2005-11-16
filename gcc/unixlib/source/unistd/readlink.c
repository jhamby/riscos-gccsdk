/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unistd/readlink.c,v $
 * $Date: 2002/11/18 15:44:17 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <errno.h>
#include <unistd.h>

int
readlink (const char *path, char *buf, size_t bufsiz)
{
  path = path;
  buf = buf;
  bufsiz = bufsiz;

  return __set_errno (ENOSYS);
}
