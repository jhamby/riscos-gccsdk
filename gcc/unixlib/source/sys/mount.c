/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/alarm.c,v $
 * $Date: 2002/09/24 21:02:38 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: alarm.c,v 1.3 2002/09/24 21:02:38 admin Exp $";
#endif

#include <sys/mount.h>
#include <errno.h>



extern int
mount (__const char *__special_file, __const char *__dir,
       __const char *__fstype, unsigned long int __rwflag,
      __const void *__data) __THROW
{
  return __set_errno (ENOSYS);
}


extern int
umount (__const char *__special_file) __THROW
{
  return __set_errno (ENOSYS);
}


extern int
umount2 (__const char *__special_file, int __flags) __THROW
{
  return __set_errno (ENOSYS);
}

