/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unistd/Attic/symlink.c,v $
 * $Date: 2002/06/15 12:59:12 $
 * $Revision: 1.1.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: symlink.c,v 1.1.2.1 2002/06/15 12:59:12 admin Exp $";
#endif

#include <errno.h>
#include <unistd.h>

int
symlink (const char *origfile, const char *newfile)
{
  origfile = origfile;
  newfile = newfile;

  return __set_errno (ENOSYS);
}
