/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/getcwd.c,v $
 * $Date: 2003/04/05 09:33:56 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getcwd.c,v 1.4 2003/04/05 09:33:56 alex Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <unixlib/os.h>
#include <swis.h>
#include <unixlib/unix.h>

#include <unixlib/local.h>
#include <pthread.h>

/* Return current working directory in Unix format ('.' and '/' swapped).  */
char *
getcwd (char *buffer, size_t size)
{
  char ro_path[_POSIX_PATH_MAX];
  char temp_buf[_POSIX_PATH_MAX];
  _kernel_oserror *err;
  int regs[10];

  PTHREAD_UNSAFE

  if (size == 0 && buffer)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  /* Try the DDE's prefix directory first, so that the program behaves as
     expected in the presence of a prefix directory. */

  if (__getenv_from_os ("Prefix$Dir", ro_path, sizeof (ro_path)) == NULL
      || *ro_path == '\0')
    {
      /* Prefix not found, so just use @ */
      ro_path[0] = '@';
      ro_path[1] = '\0';
    }

  /* Canonicalise the path */
  regs[0] = 37;
  regs[1] = (int)ro_path;
  regs[2] = (int)temp_buf;
  regs[3] = 0;
  regs[4] = 0;
  regs[5] = _POSIX_PATH_MAX;
  err = __os_swi (OS_FSControl, regs);
  if (err)
    {
      __seterr (err);
      return NULL;
    }

  return __unixify_std (temp_buf, buffer, size,
                        __RISCOSIFY_FILETYPE_NOTSPECIFIED);
}

/* BSD version of getcwd.  */

char *
getwd (char *buffer)
{
  return getcwd (buffer, _POSIX_PATH_MAX);
}
