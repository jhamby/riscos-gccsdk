/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/lstat,v $
 * $Date: 1997/10/19 19:19:46 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: lstat,v 1.1 1997/10/19 19:19:46 unixlib Exp $";
#endif

#include <sys/stat.h>

int
lstat (const char *fname, struct stat *buf)
{
  return stat (fname, buf);
}
