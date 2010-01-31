/* execv ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>

/* Execute path with arguments argv and environment from 'environ'.  */
int
execv (const char *path, char *const argv[])
{
  return execve (path, argv, environ);
}
