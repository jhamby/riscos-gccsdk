/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/link,v $
 * $Date: 1997/10/19 21:50:57 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: link,v 1.8 1997/10/19 21:50:57 unixlib Exp $";
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
