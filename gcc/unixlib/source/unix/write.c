/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/write.c,v $
 * $Date: 2001/09/11 14:16:00 $
 * $Revision: 1.2.2.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: write.c,v 1.2.2.3 2001/09/11 14:16:00 admin Exp $";
#endif

/* #define DEBUG */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <unixlib/dev.h>
#include <unixlib/unix.h>
#include <unixlib/features.h>
#include <unixlib/fd.h>

#ifdef DEBUG
#include <unixlib/os.h>
#endif

ssize_t
write (int fd, const void *buf, size_t nbytes)
{
  struct __unixlib_fd *file_desc;
  ssize_t status;

#ifdef DEBUG
  __os_print ("write(fd="); __os_prdec (fd);
  __os_print (", nbytes="); __os_prdec (nbytes); __os_print (")\r\n");
#endif

  if (nbytes == 0)
    return 0;
  if (BADF (fd))
    return __set_errno (EBADF);
  if (buf == NULL)
    return __set_errno (EINVAL);

  file_desc = &__u->fd[fd];

  /* Confirm that the file is open for writing.  */
  if ((file_desc->fflag & O_ACCMODE) == O_RDONLY)
    {
#ifdef DEBUG
      __os_print ("write(): not open for writing\r\n");
#endif
      return __set_errno (EBADF);
    }

  /* If the file is open for appending then we perform all write
     operations at the end of the file.  */
  if (file_desc->fflag & O_APPEND)
    __funcall ((*(__dev[file_desc->device].lseek)), (file_desc, 0, SEEK_END));

  /* Increment the number of times we have written to device.  */
  __u->usage.ru_oublock++;

  status = __funcall ((*(__dev[file_desc->device].write)),
		      (file_desc, buf, nbytes));

#if __FEATURE_PIPEDEV
#ifdef SIGPIPE
  /* Raise the SIGPIPE signal if we tried to write to a pipe
     or FIFO that isn't open for reading by any process.  */
  if (status == -1 && errno == EPIPE)
    raise (SIGPIPE);
#endif
#endif

  return status;
}
