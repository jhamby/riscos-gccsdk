/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/fstat,v $
 * $Date: 2000/11/08 10:22:57 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fstat,v 1.5 2000/11/08 10:22:57 admin Exp $";
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/dev.h>
#include <sys/os.h>
#include <sys/stat.h>
#include <sys/unix.h>

#include <unixlib/local.h>
#include <unixlib/swiparams.h>

int
fstat (int fd, struct stat *buf)
{
  struct __unixlib_fd *file_desc;
  int regs[10];

  if (buf == NULL)
    return __set_errno (EINVAL);

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = &__u->fd[fd];

  if (file_desc->device == DEV_RISCOS)
    {
      char *buffer;
      _kernel_oserror *err;

      if (file_desc->dflag & FILE_ISDIR)
        buffer = ((DIR *) file_desc->handle)->dd_name_can;
      else
        buffer = __fd_to_name ((int) file_desc->handle, NULL, 0);

      if (buffer == NULL)
	return __set_errno (EBADF);

      /* Get vital file statistics and use File$Path.  */
      err = os_file (OSFILE_READCATINFO, buffer, regs);
      if (err)
	{
	  __seterr (err);
	  free (buffer);
	  return __set_errno (EIO);
	}
      buf->st_ino = __get_file_ino (NULL, buffer);
      free (buffer);
    }
  else
    {
      /* Fake some stuff for the other device types.  */
      buf->st_ino = 0;
      regs[0] = regs[2] = regs[3] = regs[4] = regs[5] = 0;
    }

  buf->st_dev = file_desc->device;

  __stat (regs, buf);

  if (file_desc->device != DEV_RISCOS)
    {
      int fflag = file_desc->fflag;
      __mode_t mode = 0;

      if (fflag & O_RDONLY)
	mode |= S_IRUSR;
      if (fflag & O_WRONLY)
	mode |= S_IWUSR;
      if (fflag & O_PIPE)
	mode |= S_IFIFO;

      buf->st_mode |= mode;
    }

  return 0;
}
