/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fwrite.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fwrite.c,v 1.3 2002/02/14 15:56:36 admin Exp $";
#endif

/* #define DEBUG */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#ifdef DEBUG
#include <unixlib/os.h>
#endif

__STDIOLIB__

/* Return number of objects written.  */

size_t
fwrite (const void *data, size_t size, size_t count, FILE *stream)
{
  size_t to_write, bytes;

  PTHREAD_UNSAFE

  /* Check for any errors.  */
  if (!__validfp (stream) || !stream->__mode.__write)
    {
      (void) __set_errno (EINVAL);
      return (size_t)0;
    }

  if (ferror (stream))
    return (size_t)0;

  to_write = size * count;
  if (to_write == 0)
    return (size_t)0;

#ifdef DEBUG
  __os_print ("fwrite("); __os_prdec (stream->fd);
  __os_print ("): to_write="); __os_prdec (to_write);
#endif

  if (stream->o_base != NULL)
    {
      /* Optimisations appropriate for a buffered file.  */
      if (to_write > stream->o_cnt)
	{
#ifdef DEBUG
	  __os_print (", direct write\r\n");
#endif
	  /* We have lots of data to output. First flush the buffer,
	     then just write the rest out.  */
	  if (__flsbuf (-1, stream) < 0)
	    return (size_t)0; /* we wouldn't have written anything yet.  */

	  /* Write it out in a loop, as recommended.  */
	  while (to_write)
	    {
	      bytes = write (stream->fd, data, to_write);
	      if (bytes == -1)
		{
		  stream->__error = 1;
		  break;
		}
	      to_write -= bytes;
	      /* Increment the internal file offset.  */
	      stream->__offset += bytes;
	    }
	}
      else
	{
	  /* The data is small enough to place in the output
	     buffer.  */
#ifdef DEBUG
	  __os_print (", buffering");
#endif
	  bytes = to_write;
	  if (to_write >= 16)
	    {
	      memcpy (stream->o_ptr, data, to_write);
	      to_write -= bytes;
	    }
	  else
	    {
	      /* Try the fast case.  */
	      unsigned char *b = (unsigned char *)data;
	      unsigned char *p = stream->o_ptr;
	      while (--to_write)
	        *p++ = *b++;
	      *p++ = *b++;
	    }

	  /* Increment the file pointers. */
	  stream->o_ptr += bytes;
	  stream->o_cnt -= bytes;
	  /* If we're line buffered, look for a newline and
	     flush everything.  */
	  if (stream->__linebuf && stream->o_ptr[-1] == '\n')
	    {
#ifdef DEBUG
	      __os_print (", flushing\r\n");
#endif
	      __flsbuf (-1, stream);
	    }
#ifdef DEBUG
          else
            __os_print ("\r\n");
#endif
	}
    }
  else
    {
#ifdef DEBUG
      __os_print (", unbuffered write\r\n");
#endif
      /* Optimisations appropriate for an unbuffered file.
	 We don't have to worry about all that buffer crap :-) */
      while (to_write)
	{
	  bytes = write (stream->fd, data, to_write);
	  if (bytes == -1)
	    {
	      stream->__error = 1;
	      break;
	    }
	  to_write -= bytes;
	  stream->__offset += bytes;
	}
    }

  /* Return the number of objects actually read.  */
  return count - (to_write / size);
}
