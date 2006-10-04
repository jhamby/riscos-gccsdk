/* execle ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>

/* Execute path with all arguments after path until a NULL pointer,
   and the argument after that for environment.  */

int
execle (const char *path, const char *arg,...)
{
  const char **argv, **envp;
  unsigned int i, n = 16;
  va_list args;

  va_start (args, arg);
  argv = (const char **) malloc (n * sizeof (const char *));
  if (argv == NULL)
    return __set_errno (E2BIG);
  argv[0] = arg;
  i = 1;
  do
    {
      if (i >= n)
        {
          n *= 2;
          argv = (const char **) realloc (argv, n * sizeof (const char *));
          if (argv == NULL)
            return __set_errno (E2BIG);
        }
      argv[i] = va_arg (args, const char *);
    }
  while (argv[i++] != NULL);

  envp = va_arg (args, const char **);
  va_end (args);

  i = execve (path, (char **) argv, (char **) envp);
  free (argv);
  return i;
}
