/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fopen.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fopen.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
#endif

/* #define DEBUG */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/unix.h>

#ifdef DEBUG
#include <sys/os.h>
#endif

__STDIOLIB__

FILE *
fopen (const char *filename, const char *mode)
{
  FILE *stream;
  __io_mode m;
  __mode_t file_mode;
  int fd;

  if (filename == NULL || mode == NULL)
    {
      errno = EINVAL;
      return NULL;
    }

#ifdef DEBUG
  os_print ("fopen(fname="); os_print (filename);
  os_print (", mode="); os_print (mode); os_print ("): ");
#endif

  m = __getmode (mode);
  if (! m.__read && ! m.__write)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  /* Create new file stream.  */
  stream = __newstream ();
  if (stream == NULL)
    return NULL;

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
    fd = open (filename, file_mode | O_CREAT,
       	       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  else
    fd = open (filename, file_mode);

#ifdef DEBUG
  os_print ("fd="); os_prdec (fd); os_print ("\r\n");
#endif

  if (fd < 0)
    return NULL;

  return __stream_init (fd, stream);
}

FILE *
fdopen (int fd, const char *mode)
{
  FILE *stream;
  __io_mode m;
  int dflags;

#ifdef DEBUG
  os_print ("fdopen (fd="); os_prdec (fd);
  os_print (",mode="); os_print (mode); os_print (")\r\n");
#endif
  m = __getmode (mode);
  if (! m.__read && ! m.__write)
    return NULL;

#ifdef DEBUG
  os_print ("fdopen: here1\r\n");
#endif
  /* Verify the FD is valid and allows the access 'mode' specifies.  */
  dflags = __u->fd[fd].fflag; /* dflags = fcntl (fd, F_GETFL); */
  if (dflags == -1)
    return NULL;

#ifdef DEBUG
  os_print ("fdopen: here2\r\n");
#endif

  /* Check the access mode.  */
  if ((dflags & O_ACCMODE) == O_RDONLY && ! m.__read)
    {
      errno = EBADF;
      return NULL;
    }
  if ((dflags & O_ACCMODE) == O_WRONLY && ! m.__write)
    {
      errno = EBADF;
      return NULL;
    }

  stream = __newstream ();
  if (stream == NULL)
    return NULL;

#ifdef DEBUG
  os_print ("fdopen: here3\r\n");
#endif

  stream->__mode = m;

  return __stream_init (fd, stream);
}
