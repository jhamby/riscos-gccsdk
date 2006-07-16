/* Unix-style file-descriptor based I/O for the SCL
   Copyright (c) 1997-2005 Nick Burrett
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

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

ssize_t write (int fd, const void *buffer, size_t nbytes)
{
  FILE *handle;

  return (ssize_t) fwrite (buffer, sizeof (char), nbytes, &__iob[fd]);
}

ssize_t read (int fd, void *buffer, size_t nbytes)
{
  return (ssize_t) fread (buffer, sizeof (char), nbytes, &__iob[fd]);
}

int close (int fd)
{
  return fclose (&__iob[fd]);
}

__off_t lseek (int fd, __off_t offset, int whence)
{
  return fseek(&__iob[fd], offset, whence);
}

/* Dummy fstat for the benefit of libstdc++ with libscl */
int fstat (int fd, struct stat *buf)
{
  if (!buf)
    { 
      errno = EINVAL;
      return -1;
    }

  memset(buf, 0, sizeof(struct stat));
  buf->st_mode = S_IFREG;
  return 0;
}

