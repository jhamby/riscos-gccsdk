/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/pathconf.c,v $
 * $Date: 2000/07/15 14:52:44 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: pathconf.c,v 1.1.1.1 2000/07/15 14:52:44 nick Exp $";
#endif

/* unix.c.pathconf. Return filing system implementation details.

   Written by Nick Burrett, 13 October 1996.  */

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/dev.h>
#include <sys/unix.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

/* These functions are stub varieties that need a proper
   implementation.  */

long int
pathconf (const char *filename, int parameter)
{
  filename = filename;

  switch (parameter)
    {
    default:
      return __set_errno (EINVAL);

    case _PC_LINK_MAX:
#ifdef LINK_MAX
      return LINK_MAX;
#else
      return -1;
#endif

    case _PC_MAX_CANON:
#ifdef MAX_CANON
      return MAX_CANON;
#else
      return -1;
#endif

    case _PC_MAX_INPUT:
#ifdef MAX_INPUT
      return MAX_INPUT;
#else
      return -1;
#endif

    case _PC_NAME_MAX:
#ifdef NAME_MAX
      return NAME_MAX;
#else
      return -1;
#endif

    case _PC_PATH_MAX:
#ifdef PATH_MAX
      return PATH_MAX;
#else
      return -1;
#endif

    case _PC_PIPE_BUF:
#ifdef PIPE_BUF
      return PIPE_BUF;
#else
      return -1;
#endif

    case _PC_CHOWN_RESTRICTED:
#ifdef _POSIX_CHOWN_RESTRICTED
      return _POSIX_CHOWN_RESTRICTED;
#else
      return -1;
#endif

    case _PC_NO_TRUNC:
#ifdef _POSIX_NO_TRUNC
      return _POSIX_NO_TRUNC;
#else
      return -1;
#endif

    case _PC_VDISABLE:
#ifdef _POSIX_VDISABLE
      return _POSIX_VDISABLE;
#else
      return -1;
#endif
    }
}

long int
fpathconf (int fd, int selection)
{
  char filename[_POSIX_PATH_MAX];

  if (BADF (fd))
    return __set_errno (EBADF);

  if (__u->fd[fd].device == DEV_RISCOS)
    {
      if (__fd_to_name (fd, filename, sizeof (filename)) == NULL)
	return __set_errno (EBADF);
      return pathconf (filename, selection);
    }
  else
    return pathconf (NULL, selection);
}
