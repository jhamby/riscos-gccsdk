/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unistd/chown.c,v $
 * $Date: 2005/11/16 03:15:42 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define IGNORE(x) x = x

int
lchown (const char *file, uid_t owner, gid_t group)
{
  IGNORE (file);
  IGNORE (owner);
  IGNORE (group);
  return __set_errno (ENOSYS);
}
