/* unistd.h emulation functions.

   For use with the GNU compilers and the SharedCLibrary.
   Copyright (c) 1999 Nick Burrett.  */

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

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
