/* UnixLib fread() implementation.
   Copyright 2000-2008 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include <pthread.h>
#include <internal/unix.h>

/* Return number of objects read.  */
size_t
fread (void *data, size_t size, size_t count, FILE *stream)
{
  size_t to_read;
  ssize_t bytes;

  PTHREAD_UNSAFE

  /* Check for any errors.  */
  if (!__validfp (stream) || !stream->__mode.__bits.__read)
    {
      (void) __set_errno (EINVAL);
      return (size_t) 0;
    }

  if (feof (stream) || ferror (stream))
    return (size_t) 0;

  to_read = size * count;
  if (to_read == 0)
    return (size_t) 0;

  /* If we don't do this ungetc() followed by fread() screws up! */
  if (stream->__pushedback == 1)
  {
    stream->__pushedback = 0;
    stream->i_cnt = stream->__pushedi_cnt;

    *((char *)data) = stream->__pushedchar;
    data = (void *)((unsigned int)data + 1);
    to_read--;
  }

  if (stream->i_base != NULL)
    {
      /* Optimisations appropriate for a buffered file.  */

      /* Take the first set of data out of the file buffer.  */
      bytes = (to_read > stream->i_cnt) ? stream->i_cnt : to_read;
      memcpy (data, stream->i_ptr, bytes);
      data = (void *)((unsigned int)data + bytes);
      /* Increment the file pointers. */
      stream->i_cnt -= bytes;
      stream->i_ptr += bytes;
      to_read -= bytes;

      if (to_read)
        {
          /* Read the rest of the data straight from the file.  */
          if (stream->__string_istream)
            bytes = 0;
          else
            bytes = read (stream->fd, data, to_read);
          if (bytes <= 0)
            {
              if (bytes == 0)
                stream->__eof = 1;
              else
                stream->__error = 1;

              return count - (to_read / size);
            }
          data = (void *)((unsigned int)data + bytes);
          to_read -= bytes;
          /* Fix the file offset pointer. Also fix the buffer
             pointers for a refresh on the next read.  */
          stream->__offset += bytes;
          stream->i_cnt = 0;
        }
    }
  else
    {
      /* Optimisations appropriate for an unbuffered file.
         We don't have to worry about all that buffer crap :-) */
      if (stream->__string_istream)
        bytes = 0;
      else
        bytes = read (stream->fd, data, to_read);
      if (bytes == 0)
        stream->__eof = 1;
      else if (bytes == -1)
        stream->__error = 1;
      else
        to_read -= bytes;
      stream->__offset += bytes;
    }

  /* Return the number of objects actually read.  */
  return count - (to_read / size);
}
