/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/lstat.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: lstat.c,v 1.2 2001/01/29 15:10:22 admin Exp $";
#endif

#include <sys/stat.h>

int
lstat (const char *fname, struct stat *buf)
{
  return stat (fname, buf);
}

int
lstat64 (const char *fname, struct stat *buf)
{
  return stat (fname, buf);
}
