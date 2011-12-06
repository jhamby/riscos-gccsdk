/* Unix-style file-descriptor based I/O for the SCL
   Copyright (c) 1997 Peter Burwood
   Copyright (c) 1998-2005 Nick Burrett
   Copyright (c) 2011 UnixLib Developers.  */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#include <internal/os.h>

int
isatty (int fd)
{
  if ((unsigned)fd >= __FD_SOCKET_OFFSET)
    return __set_errno (EBADF);

  /* If we cannot seek on fp, the file is probably (?) a terminal,
     and so interactive.  */
  FILE *fp = &__iob[fd];
  if (fseek (fp, 0, SEEK_CUR) == -1)
    return 1;

  /* We can seek, so we are a normal file. Does the OS think that fp
     is interactive ?  */
  int status;
  const _kernel_oserror *err;
  if ((err = SWI_OS_Args_GetFileHandleStatus (fp->__file, &status)) != NULL)
    return __ul_seterr (err, EOPSYS);
  /* Bit 11 means unallocated, if unset, then bit 3 means interactive.  */
  if ((status & 0x800) && (fp == stdin || fp == stdout || fp == stderr))
    return 1;
  else
    return (status & 0x808) == 8;
}

