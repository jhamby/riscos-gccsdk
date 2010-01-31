/* Return a canonicalised RISC OS filename.
   Copyright (c) 2003, 2005, 2008 UnixLib Developers.  */

#include <stdlib.h>
#include <swis.h>
#include <errno.h>

#include <unixlib/local.h>
#include <internal/os.h>
#include <pthread.h>

/* Return canonicalised RISC OS filename corresponding to a RISC OS file
   descriptor RISCOS_FD, or NULL if failed to canonicalise filename. The
   filename is copied into BUF.  If BUF is NULL, then allocate space as
   necessary.  This is an internal function and thus does not set errno
   since that is the responsibility of the caller.

   This function is thread safe.  */
char *
__fd_to_name (int riscos_fd, char *buf, size_t buflen)
{
  _kernel_oserror *err;
  int regs[10];

  /* Check illegal argument combination.  */
  if (buflen == 0 && buf != NULL)
    return NULL;

  regs[0] = 7;
  regs[1] = riscos_fd;

  if (buf != NULL)
    regs[5] = buflen;
  else
    {
      /* Find required buffer size. */
      regs[2] = 0;
      regs[5] = 0;

      err = __os_swi (OS_Args, regs);
      if (err)
	/* Do not return here, fall through.  */
	__ul_seterr (err, 0);
      else
	{
	  /* regs[5] is now -( length of filename - oldreg[5] ) */
	  regs[5] = 1 - regs[5];
	  buf = (char *) malloc (regs[5]);
	  /* Set buflen to 0 to enable free () below on error.  */
	  buflen = 0;
	}
    }

  regs[2] = (int) buf;

  /* Finally, get the filename into the buffer.  */
  if (buf != NULL && (err = __os_swi (OS_Args, regs)) != NULL)
    {
      __ul_seterr (err, 0);
      if (buflen == 0)
	free (buf);
      buf = NULL;
    }

  return buf;
}
