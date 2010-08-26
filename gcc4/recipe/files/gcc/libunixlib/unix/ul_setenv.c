/* setenv () for UnixLib
   Copyright (c) 2005-2010 UnixLib Developers.  */

#include <errno.h>
#include <string.h>

#include <internal/local.h>
#include <internal/unix.h>

int
setenv (const char *name, const char *value, int replace)
{
  if (strchr (name, '='))
    return __set_errno (EINVAL);

  if (strchr (name, '$'))
    return __addenv_to_os (name, value, replace);
  else
    return __addenv_to_env (NULL, name, value, replace);
}

