/* Get the current working directory.
   Copyright (c) 2004-2011 UnixLib Developers.  */

#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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
  PTHREAD_UNSAFE

  if (size == 0 && buffer != NULL)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  /* Try the DDE's prefix directory first, so that the program behaves
     as expected in the presence of a prefix directory.  */
  const char *ro_path;
  int free_ro_path;
  if ((ro_path = __get_dde_prefix ()) == NULL)
    {
      /* Prefix not found, so just use @.  */
      ro_path = "@";
      free_ro_path = 0;
    }
  else
    free_ro_path = 1;

  /* Canonicalise the path.  */
  char temp_buf[_POSIX_PATH_MAX];
  const _kernel_oserror *err = SWI_OS_FSControl_Canonicalise (ro_path, NULL,
							      temp_buf, sizeof (temp_buf),
							      NULL);
  free ((void *)ro_path);
  if (err)
    {
      __ul_seterr (err, EOPSYS);
      return NULL;
    }

  return __unixify_std (temp_buf, buffer, size,
                        __RISCOSIFY_FILETYPE_NOTSPECIFIED);
}

