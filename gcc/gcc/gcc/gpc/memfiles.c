/* Memory-buffered files
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA. */

/*
 * Author:  Peter Gerwinski <peter@gerwinski.de>
 */


/* Currently, this only implements read-only access through the
 * fread() function family. Someone else can generalize this to
 * support read-write access and/or fprintf()-style I/O.
 */


#include "config.h"
#include "gbe.h"

#ifdef EGCS
#include "system.h"
#else
extern char *xmalloc PROTO ((size_t));
#endif

#include "memfiles.h"

#include <errno.h>
#include <sys/stat.h>

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_UNISTD_H
# include <sys/types.h>
# include <unistd.h>
#else
# define R_OK 4
# define W_OK 2
# define X_OK 1
#endif


/* Open a memory-buffered file.
 */
MEMFILE *
mopen (name, mode)
     const char *name, *mode;
{
  FILE *the_file = fopen (name, mode);
  if (the_file)
    {
      MEMFILE *s;
      struct stat finfo;
      int error = 0;

      if (fstat (fileno (the_file), &finfo) != 0)
        return NULL_PTR;
      s = (MEMFILE *) xmalloc (sizeof (MEMFILE));
      s->size = finfo.st_size;
      s->buffer = (char *) xmalloc (s->size);
      if (fread (s->buffer, 1, s->size, the_file) != s->size)
        error = 1;
      s->curpos = 0;
      fclose (the_file);

      if (error)
        {
          free (s);
          return NULL_PTR;
        }
      else
        return s;
    }
  else
    return NULL_PTR;
}


/* Close a MEMFILE opened by mopen().
 */
int
mclose (stream)
     MEMFILE *stream;
{
  if (stream)
    {
      free (stream->buffer);
      free (stream);
      return 0;
    }
  else
    {
      errno = EBADF;
      return EOF;
    }
}


/* Read from a MEMFILE.
 */
size_t
mread (ptr, member_size, members, stream)
   void *ptr;
   size_t member_size, members;
   MEMFILE *stream;
{
  size_t total_size = member_size * members;
  if (stream->curpos + total_size > stream->size)
    {
      total_size = stream->size - stream->curpos;
      members = total_size / member_size;
    }
  memcpy (ptr, stream->buffer + stream->curpos, total_size);
  stream->curpos += total_size;
  return members;
}


/* Get the file position.
 */
long
mtell (stream)
     MEMFILE *stream;
{
  return stream->curpos;
}


/* Jump to a file position.
 */
int
mseek (stream, offset, whence)
     MEMFILE *stream;
     long offset;
     int whence;
{
  switch (whence)
    {
      case SEEK_SET:
        {
          stream->curpos = offset;
          break;
        }
      case SEEK_CUR:
        {
          stream->curpos += offset;
          break;
        }
      case SEEK_END:
        {
          stream->curpos = stream->size + offset;
          break;
        }
      default:
        {
          errno = EINVAL;
          return -1;
        }
    }
  if (stream->curpos > stream->size)
    {
      /* `man fseek' does not speak about error handling here.
       */
      stream->curpos = stream->size;
    }
  return 0;
}
