/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/mount.c,v $
 * $Date: 2004/10/08 11:56:13 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: mount.c,v 1.1 2004/10/08 11:56:13 peter Exp $";
#endif

#include <sys/mount.h>
#include <errno.h>



extern int
mount (__const char *__special_file, __const char *__dir,
       __const char *__fstype, unsigned long int __rwflag,
      __const void *__data)
{
  return __set_errno (ENOSYS);
}


extern int
umount (__const char *__special_file)
{
  return __set_errno (ENOSYS);
}


extern int
umount2 (__const char *__special_file, int __flags)
{
  return __set_errno (ENOSYS);
}

