/* fstat() for SCL.
   Copyright (c) 2012 UnixLib Developers.  */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <internal/local.h>
#include <internal/os.h>

int
fstat (int fd, struct stat *buf)
{
  const FILE *fp = &__iob[fd];

  const char *buffer = __canonicalise_handle (fp->__file);
  if (buffer == NULL)
    return __set_errno (EBADF);

  /* Get vital file statistics.  */
  unsigned objtype, loadaddr, execaddr, objlen, attr;
  const _kernel_oserror *err = SWI_OS_File_ReadCatInfo (buffer, &objtype,
							&loadaddr, &execaddr,
							&objlen, &attr);
  free ((void *)buffer);
  if (err)
    return __ul_seterr (err, EIO);

  /* OS_File ReadCatInfo returns the allocated size of the file, but we want
     the current extent of the file */
  if ((err = SWI_OS_Args_GetExtent (fp->__file, &objlen)) != NULL)
    return __ul_seterr (err, EIO);

  buf->st_ino = 0;
  buf->st_dev = 0;
  return __stat (objtype, loadaddr, execaddr, objlen, attr, buf);
}
 
int
fstat64 (int fd, struct stat64 *buf)
{
  /* struct stat64 is currently the same as struct stat.  */
  return fstat (fd, (struct stat *)buf);
}

