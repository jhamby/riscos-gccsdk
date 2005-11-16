/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/mount.c,v $
 * $Date: 2005/04/07 18:34:11 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <sys/mount.h>
#include <errno.h>


int
mount (__const char *__special_file, __const char *__dir,
       __const char *__fstype, unsigned long int __rwflag,
      __const void *__data)
{
  return __set_errno (ENOSYS);
}


int
umount (__const char *__special_file)
{
  return __set_errno (ENOSYS);
}


int
umount2 (__const char *__special_file, int __flags)
{
  return __set_errno (ENOSYS);
}
