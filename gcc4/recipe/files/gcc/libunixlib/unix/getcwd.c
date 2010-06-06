/* Get the current working directory.
   Copyright (c) 2004-2010 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <swis.h>

#include <unixlib/local.h>
#include <internal/os.h>
#include <internal/unix.h>
#include <internal/local.h>
#include <pthread.h>

/* Return current working directory in Unix format
   ('.' and '/' swapped).  */
char *
getcwd (char *buffer, size_t size)
{
  const char *ro_path;
  int free_ro_path;
  char temp_buf[_POSIX_PATH_MAX];
  _kernel_oserror *err;
  int regs[10];

  PTHREAD_UNSAFE

  if (size == 0 && buffer != NULL)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  /* Try the DDE's prefix directory first, so that the program behaves
     as expected in the presence of a prefix directory.  */
  if ((ro_path = __get_dde_prefix ()) == NULL)
    {
      /* Prefix not found, so just use @.  */
      ro_path = "@";
      free_ro_path = 0;
    }
  else
    free_ro_path = 1;

  /* Canonicalise the path.  */
  regs[0] = 37;
  regs[1] = (int)ro_path;
  regs[2] = (int)temp_buf;
  regs[3] = 0;
  regs[4] = 0;
  regs[5] = sizeof(temp_buf);
  err = __os_swi (OS_FSControl, regs);
  if (free_ro_path)
    free ((void *)ro_path);
  if (err)
    {
      __ul_seterr (err, EOPSYS);
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
