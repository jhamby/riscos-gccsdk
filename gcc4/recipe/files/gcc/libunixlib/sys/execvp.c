/* execvp ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <unistd.h>

/* #define DEBUG 1 */
#ifdef DEBUG
#  include <sys/debug.h>
#  include <internal/unix.h>
#  include <pthread.h>
#endif

/* Execute file, searching in the 'PATH' environment variable if it
   contains no slashes, with arguments argv and environment from
   'environ'.  */
int
execvp (const char *name, char *const argv[])
{
#ifdef DEBUG
  PTHREAD_UNSAFE

  if (environ != NULL)
    {
      int x;

      debug_printf ("-- execvp:\n");
      for (x = 0; environ[x] != NULL; ++x)
	debug_printf ("   environ[%d]: %s\n", x, environ[x]);
    }
  else
    debug_printf ("-- execvp: environ == NULL\n");
#endif

  /* Path searching has not been implemented.  */
  return execve (name, argv, environ);
}
