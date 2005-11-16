/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/lstat.c,v $
 * $Date: 2004/06/12 08:59:48 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

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
