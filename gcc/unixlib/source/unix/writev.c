/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/writev.c,v $
 * $Date: 2004/11/28 21:31:35 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: writev.c,v 1.5 2004/11/28 21:31:35 joty Exp $";
#endif

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include <sys/uio.h>
#include <unixlib/dev.h>
#include <sys/types.h>
#include <unixlib/unix.h>

#include <unixlib/fd.h>
#include <pthread.h>

/* Write data pointed by the buffers described by VECTOR, which
   is a vector of COUNT `struct iovec's, to file descriptor FD.
   The data is written in the order specified.  */

ssize_t
writev (int fd, const struct iovec *vector, int count)
{
  struct __unixlib_fd *file_desc;
  size_t bytes, bytes_written;
  int i;
  unsigned int device;

  PTHREAD_UNSAFE_CANCELLATION

  if (count <= 0)
    return __set_errno (EINVAL);
  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);
  if ((file_desc->fflag & O_ACCMODE) == O_RDONLY)
    return __set_errno (EBADF);

  device = file_desc->devicehandle->type;

  /* If the file is open for appending then we perform all write
     operations at the end of the file.  */
  if (file_desc->fflag & O_APPEND)
    dev_funcall (device, lseek, (file_desc, 0, SEEK_END));

  /* Write each buffer, recording how many bytes were written.  */
  bytes_written = 0;
  for (i = 0; i < count; ++i)
    if (vector[i].iov_len > 0)
      {
	__u->usage.ru_oublock++;
	bytes = dev_funcall (device, write,
			   (file_desc, vector[i].iov_base, vector[i].iov_len));
	/* If we failed on the first write, then return -1, otherwise return
	   the number of bytes we have written.  */
	if (bytes == -1)
	  {
	    /* Raise the SIGPIPE signal if we tried to write to a pipe
	       or FIFO that isn't open for reading by any process.  */
	    if (errno == EPIPE)
	      raise (SIGPIPE);
	    return bytes_written ? bytes_written : -1;
	  }
	bytes_written += bytes;
	/* If we did not write enough bytes, then give up.  */
	if (bytes != vector[i].iov_len)
	  return bytes_written;
      }

  return bytes_written;
}
