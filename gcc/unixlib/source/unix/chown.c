/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/chown,v $
 * $Date: 1997/10/19 21:50:52 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: chown,v 1.7 1997/10/19 21:50:52 unixlib Exp $";
#endif

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
  /* FIXME Should we say ENOSYS ?  */
  return 0;
}
