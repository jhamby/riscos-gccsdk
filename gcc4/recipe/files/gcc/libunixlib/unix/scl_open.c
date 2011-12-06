/* Unix-style file-descriptor based I/O for the SCL
   Copyright (c) 1997 Peter Burwood
   Copyright (c) 1998-2005 Nick Burrett
   Copyright (c) 2011 UnixLib Developers.  */

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <internal/os.h>
#include <internal/swiparams.h>

int
open (const char *file, int oflag, ...)
{
  if (file == NULL)
    return __set_errno (EINVAL);

  /* Mode is actually ignored.  */
  int mode;
  if (oflag & O_CREAT)
    {
      va_list ap;
      va_start (ap, oflag);
      mode = va_arg (ap, int);
      va_end (ap);
    }
  else
    mode = 0777;

  const _kernel_oserror *err;
  unsigned objtype;
  if ((err = SWI_OS_File_ReadCatInfo (file, &objtype, NULL, NULL,
				      NULL, NULL)) != NULL)
    return __ul_seterr (err, EOPSYS);

  if (objtype)
    {
      /* If the file already exists and exclusive access requested, then error.  */
      if ((oflag & (O_EXCL | O_CREAT)) == (O_EXCL | O_CREAT))
	return __set_errno (EEXIST);
      /* Directory ? */
      if (objtype == 2)
	return __set_errno (EISDIR);
    }

  char cmode[4];
  cmode[1] = '\0';
  cmode[2] = '\0';
  cmode[3] = '\0';
  switch (oflag & O_ACCMODE)
    {
      case O_RDONLY:
	cmode[0] = 'r';      
        break;

      case O_WRONLY:
        if (oflag & O_APPEND)
          cmode[0] = 'a';
        else
          cmode[0] = 'w';
        break;

      case O_RDWR:
        if (oflag & O_APPEND)
          cmode[0] = 'a';
        else if (oflag & O_TRUNC)
          cmode[0] = 'w';
        else
          cmode[0] = 'r';
	cmode[1] = '+';
        break;

      default:
        return __set_errno (EINVAL);
    }

  FILE *fp = fopen (file, cmode);
  if (fp == NULL)
    return -1;
  return fp - &__iob[0];
}

