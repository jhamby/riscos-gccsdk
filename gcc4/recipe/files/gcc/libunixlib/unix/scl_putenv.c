/* putenv () for SCL
   Copyright (c) 2010 UnixLib Developers.  */

#include <alloca.h>
#include <stdlib.h>
#include <string.h>

#include <internal/local.h>

/* Put STRING, which is of the form "NAME=VALUE", in the environment.
   If there is no '=' in STRING, then remove it from the environment.  */
int
putenv (char *string)
{
  const char *equal = strchr (string, '=');
  if (!equal)
    return unsetenv (string);

  char *name = alloca (equal - string + 1);
  memcpy(name, string, equal - string);
  name[equal - string] = '\0';

  return __addenv_to_os (name, equal + 1, 1);
}
