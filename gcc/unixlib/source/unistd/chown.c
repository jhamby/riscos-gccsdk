/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unistd/chown.c,v $
 * $Date: 2002/11/18 15:44:17 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#define IGNORE(x) x = x

int
chown (const char *file, uid_t owner, gid_t group)
{
  IGNORE (file);
  IGNORE (owner);
  IGNORE (group);
  return __set_errno (ENOSYS);
}
