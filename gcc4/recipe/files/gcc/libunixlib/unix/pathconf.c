/* pathconf (), fpathconf ()
 * Return filing system implementation details.
 * Written by Nick Burrett, 13 October 1996.
 * Copyright (c) 1996-2011 UnixLib Developers
 */

#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stddef.h>

#include <internal/dev.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <internal/os.h>
#include <internal/unix.h>
#include <pthread.h>

/* These functions are stub varieties that need a proper
   implementation.  */

long int
pathconf (const char *filename, int parameter)
{
  switch (parameter)
    {
#ifdef LINK_MAX
      case _PC_LINK_MAX:
        return LINK_MAX;
#endif

#ifdef MAX_CANON
      case _PC_MAX_CANON:
        return MAX_CANON;
#endif

#ifdef MAX_INPUT
      case _PC_MAX_INPUT:
        return MAX_INPUT;
#endif

#ifdef NAME_MAX
      case _PC_NAME_MAX:
        return NAME_MAX;
#endif

#ifdef PATH_MAX
      case _PC_PATH_MAX:
        return PATH_MAX;
#endif

#ifdef PIPE_BUF
      case _PC_PIPE_BUF:
        return PIPE_BUF;
#endif

#ifdef _POSIX_CHOWN_RESTRICTED
      case _PC_CHOWN_RESTRICTED:
        return _POSIX_CHOWN_RESTRICTED;
#endif

#ifdef _POSIX_NO_TRUNC
      case _PC_NO_TRUNC:
        return _POSIX_NO_TRUNC;
#endif

#ifdef _POSIX_VDISABLE
      case _PC_VDISABLE:
        return _POSIX_VDISABLE;
#endif
    }

  return __set_errno (EINVAL);
}

long int
fpathconf (int fd, int selection)
{
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  if (getfd (fd)->devicehandle->type == DEV_RISCOS)
    {
      char filename[_POSIX_PATH_MAX];
      const _kernel_oserror *err;
      if ((err = SWI_OS_Args_Canonicalise (fd, filename, sizeof (filename),
					   NULL)) != NULL)
	return __ul_seterr (err, EBADF);

      return pathconf (filename, selection);
    }

  return pathconf (NULL, selection);
}
