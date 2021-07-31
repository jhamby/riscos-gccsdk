/* setenv () for SCL
   Copyright (c) 2010 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <internal/local.h>

int
setenv (const char *name, const char *value, int replace)
{
  if (strchr (name, '='))
    return __set_errno (EINVAL);

  return __addenv_to_os (name, value, replace);
}

