/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/freopen.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: freopen.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
#endif

/* #define DEBUG */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unixlib/local.h>
#ifdef DEBUG
#include <sys/os.h>
#endif

__STDIOLIB__


FILE *
freopen (const char *filename, const char *mode, FILE *stream)
{
  __io_mode m;
  __mode_t file_mode;
  int fd;

  /* Close the file if it is open.  */
  if (!__validfp (stream))
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  if (filename == NULL || mode == NULL)
    {
      errno = EINVAL;
      return NULL;
    }

#ifdef DEBUG
  os_print ("freopen(file="); os_print (filename);
  os_print (",mode="); os_print (mode);
  os_print (",fd="); os_prdec (stream->fd);
  os_print (")\r\n");
#endif

  m = __getmode (mode);
  if (! m.__read && ! m.__write)
    {
      fclose (stream);
      (void) __set_errno (EINVAL);
      return NULL;
    }


  /* Flush the stream.  */
  if (stream->__mode.__write)
    __flsbuf (EOF, stream);

  /* Reset the file buffers.  */
  stream->i_ptr = stream->i_base;
  stream->i_cnt = 0;
  stream->o_cnt = stream->__bufsize;
  stream->o_ptr = stream->o_base;
  /* Discard the pushed back character.  */
  stream->__pushedback = 0;
  /* Since we are reopening, we are no longer at EOF.  */
  stream->__eof = 0;

  stream->__mode = m;

  if (m.__read && m.__write)
    file_mode = O_RDWR;
  else
    file_mode = m.__read ? O_RDONLY : O_WRONLY;

  if (m.__append)
    file_mode |= O_APPEND;
  if (m.__truncate)
    file_mode |= O_TRUNC;

  if (m.__create)
    fd = __reopen (stream->fd, filename, file_mode | O_CREAT,
       	       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  else
    fd = __reopen (stream->fd, filename, file_mode);

  if (fd < 0)
    {
      fclose (stream);
      return NULL;
    }

  return stream;
}
