/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/link.c,v $
 * $Date: 2002/01/31 14:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: link.c,v 1.2.2.1 2002/01/31 14:32:04 admin Exp $";
#endif

#include <errno.h>
#include <unistd.h>

int
link (const char *origfile, const char *newfile)
{
  origfile = origfile;
  newfile = newfile;

  return __set_errno (ENOSYS);
}

int
symlink (const char *origfile, const char *newfile)
{
  origfile = origfile;
  newfile = newfile;

  return __set_errno (ENOSYS);
}

int
readlink (const char *path, char *buf, size_t bufsiz)
{
  path = path;
  buf = buf;
  bufsiz = bufsiz;

  return __set_errno (ENOSYS);
}
