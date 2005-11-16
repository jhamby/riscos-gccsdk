/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unistd/link.c,v $
 * $Date: 2002/11/18 15:44:17 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <errno.h>
#include <unistd.h>

int
link (const char *origfile, const char *newfile)
{
  origfile = origfile;
  newfile = newfile;

  return __set_errno (ENOSYS);
}
