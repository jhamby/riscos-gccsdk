/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/getdelim.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* c.getdelim. Written by Nick Burrett, 27 October 1996.  */

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getdelim.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
#endif

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>

/* Read up to (and including) a terminator from stream into *lineptr
   (and null-terminate it). *lineptr is a pointer returned from malloc
   (or NULL), pointing to *n charcters of space. It is realloc'd as
   necessary. Returns the number of characters read (not including the
   null terminator), or -1 on error or EOF.  */

ssize_t
getdelim (char **lineptr, size_t * n, int terminator, FILE * stream)
{
  char *line, *p;
  size_t size, copy;

  /* Validity check.  */
  if (!__validfp (stream) || lineptr == NULL || n == NULL)
    return __set_errno (EINVAL);

  if (ferror (stream))
    return -1;

  /* Make sure we have a line buffer to start with.  */
  if (*lineptr == NULL || *n < 2)
    {
      line = realloc (*lineptr, MAX_CANON);
      if (line == NULL)
	return -1;
      *lineptr = line;
      *n = MAX_CANON;
    }

  line = *lineptr;
  size = *n;

  copy = size;
  p = line;

  for (;;)
    {
      size_t len;

      /* Get the characters, byte by byte, into buffer line until
	 we reach the termination character, or we run out of buffer
	 space.  */
      while (--copy > 0)
	{
	  int c = getc (stream);
	  if (c == EOF)
	    {
	      if (p == *lineptr)
		return -1;
	      /* Return a partial line since we got an error in the middle.  */
	      *p = '\0';
	      return p - *lineptr;
	    }
	  else if ((*p++ = c) == terminator)
	    {
	      *p = '\0';
	      return p - *lineptr;
	    }
	}

      /* Need to enlarge the line buffer.  */
      len = p - line;
      size *= 2;
      line = realloc (line, size);
      /* Bit of a memory problem, break out of while() and return
	 what we managed to obtain.  */
      if (line == NULL)
	break;
      *lineptr = line;
      *n = size;
      p = line + len;
      copy = size - len;
    }

  if (p == *lineptr)
    return -1;

  *p = '\0';
  return p - *lineptr;
}

ssize_t
getline (char **lineptr, size_t * n, FILE * stream)
{
  return getdelim (lineptr, n, '\n', stream);
}
