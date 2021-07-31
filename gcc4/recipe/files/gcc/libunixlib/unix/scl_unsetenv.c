/* unsetenv () for SCL
   Copyright (c) 2010 UnixLib Developers.  */

#include <errno.h>
#include <string.h>

#include <internal/local.h>

int
unsetenv (const char *name)
{
  if (name == NULL || strchr (name, '='))
    return __set_errno (EINVAL);

  return __remenv_from_os (name);
}

