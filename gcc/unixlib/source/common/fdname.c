/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/fdname.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fdname.c,v 1.2.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

#include <stdlib.h>
#include <unixlib/local.h>
#include <unixlib/os.h>
#include <swis.h>
#include <errno.h>

/* Return canonicalised RISCOS filename corresponding to a RISC OS file
   descriptor RISCOS_FD, or NULL if failed to canonicalise filename. The
   filename is copied into BUF.  If BUF is NULL, then allocate space as
   necessary. This is an internal function and thus does not set errno
   since that is the responsibility of the caller.  */

char *
__fd_to_name (int riscos_fd, char *buf, size_t buflen)
{
  int save = errno;
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
	__seterr (err);		/* Do not return here, fall through.  */
      else
	{
	  /* regs[5] is now -( length of filename - oldreg[5] ) */
	  regs[5] = 1 - regs[5];
	  buf = malloc (regs[5]);
	  /* Set buflen to 0 to enable free () below on error.  */
	  buflen = 0;
	}
    }

  regs[2] = (int) buf;

  /* Finally, get the filename into the buffer.  */
  if (buf != NULL && (err = __os_swi (OS_Args, regs)) != NULL)
    {
      __seterr (err);
      if (buflen == 0)
	free (buf);
      buf = NULL;
    }

  /* Restore errno so callers do not get upset.  */
  (void) __set_errno (save);

  return buf;
}
