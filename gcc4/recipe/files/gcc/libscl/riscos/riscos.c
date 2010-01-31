/* Unix-style file-descriptor based I/O for the SCL
   Copyright (c) 1997 Peter Burwood
   Copyright (c) 1998-2005 Nick Burrett
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include <errno.h>

#define __RISCOSIFY
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "kernel.h"

/* Get O_xxx macros and ensure same as UnixLib, so that libgnat
   is compatible with UnixLib and stubs. */
#include "fcntl.h"

/* Get TCxxx macros and ensure same as UnixLib, so that libgnat
   is compatible with UnixLib and stubs.  */
#include "termio.h"

#if 0
static FILE *fd_to_file (int fd)
{
  if (BADF (fd))
    return (FD_FILE(fd));
  else
    return NULL;
}
#endif

int close (int fd)
{
  if (BADF (fd))
    {
      errno = EBADF;
      return -1;
    }
  return fclose (FD_FILE (fd));
}

int (fileno) (FILE *fp)
{
  return fileno (fp);
}

int unlink(char *file)
{
  return remove (file);
}


int open (char *filename, int oflag, ...)
{
  va_list ap;
  int mode;
  FILE *fp;
  char cmode[4];
  int regs[6];
  os_error *err;

  /* mode is actually ignored */
  if (oflag & O_CREAT)
    {
      va_start (ap, oflag);
      mode = va_arg (ap, int);
      va_end (ap);
    }
  else
    mode = 0777;

  filename = __riscosify_scl (filename, oflag & (O_CREAT | O_OMASK));

  if ((err = os_file (0x05, filename, regs)) != NULL)
    {
      __seterr (err);
      return -1;
    }

  if (regs[0])
    {
      /* if the file already exists and exclusive access requested, then error */
      if ((oflag & (O_EXCL | O_CREAT)) == (O_EXCL | O_CREAT))
        {
          errno = EEXIST;
          return -1;
        }
      /* directory ? */
      if (regs[0] == 2)
        {
	  errno = EISDIR;
	  return -1;
        }
    }

  cmode[1] = '\0';
  cmode[2] = '\0';
  cmode[3] = '\0';
  switch (oflag)
    {
      case O_RDONLY:
	cmode[0] = 'r';      
        break;
      case O_WRONLY:
        if (oflag & O_APPEND)
          cmode[1] = 'a';
        else
          cmode[1] = 'w';
        break;
      case O_RDWR:
	cmode[1] = '+';
        if (oflag & O_APPEND)
          cmode[0] = 'a';
        else if (oflag & O_TRUNC)
          cmode[0] = 'w';
        else
          cmode[0] = 'r';
        break;
      default:
        errno = EINVAL;
        return -1;
    }

  if (oflag & O_BINARY)
    {
      strcat (cmode, "b");
    }

  fp = fopen (filename, cmode);
  if (fp == NULL)
    {
      __seterr ((os_error*)_kernel_last_oserror());
      return -1;
    }
  return fileno(fp);
}

/* convert file descriptor fd to FILE*. Should check compatibility of mode.  */
FILE *fdopen (int fd, const char *mode)
{
  if (BADF (fd))
    {
      errno = EBADF;
      return (FILE*)-1;
    }
  return FD_FILE (fd);
}

int read (int fd, void *data, int nbyte)
{
  if (BADF (fd))
    {
      errno = EBADF;
      return -1;
    }
  return fread (data, 1, nbyte, FD_FILE (fd));
}

int write (int fd, void *data, int nbyte)
{
  if (BADF (fd))
    {
      errno = EBADF;
      return -1;
    }
  return fwrite (data, 1, nbyte, FD_FILE (fd));
}

int isatty (int fd)
{
  FILE *fp;
  int regs[6];

  if (BADF (fd))
    {
      errno = EBADF;
      return -1;
    }

  /* If fp == NULL, say non-interactive... */
  if ((fp = FD_FILE (fd)) == NULL)
    return 0;

  /* If we cannot seek on fp, the file is probably (?) a terminal,
     and so interactive. */
  if (fseek (fp, 0, SEEK_CUR) != NULL)
    return 1;

  /* We can seek, so we are a normal file. Does the OS think that fp
     is interactive? */
  os_args (254, fp->__file, 0, regs);
  /* bit 11 means unallocated, if unset, then bit 3 means interactive */
  if ((regs[0] & 0x800) && (fp == stdin || fp == stdout || fp == stderr))
    return 1;
  else
    return ((regs[0] & 0x808) == 8);
}

int (fisatty) (FILE *fp)
{
  return isatty (fileno (fp));
}

int ioctl (int fd, int request, void *arg)
{
  if (fd == 0 && (request == TCGETA || request == TCSETA))
    return 0;
  errno = EINVAL;
  return -1;
}

int getpid (void)
{
  return 2;
}
