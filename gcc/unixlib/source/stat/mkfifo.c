/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stat/Attic/mkfifo.c,v $
 * $Date: 2002/06/15 12:59:11 $
 * $Revision: 1.1.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mkfifo.c,v 1.1.2.1 2002/06/15 12:59:11 admin Exp $";
#endif

#include <errno.h>
#include <sys/stat.h>

int
mkfifo (const char *path, mode_t mode)
{
  path = path;
  mode = mode;

  return __set_errno (ENOSYS);
}
