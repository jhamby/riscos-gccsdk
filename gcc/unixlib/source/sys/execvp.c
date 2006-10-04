/* execvp ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <unistd.h>

/* #define DEBUG 1 */
#ifdef DEBUG
#include <unixlib/os.h>
#include <pthread.h>
#endif

/* Execute file, searching in the 'path' environment variable if it
   contains no slashes, with arguments argv and environment from
   'environ'.  */
int
execvp (const char *name, char *const argv[])
{
#ifdef DEBUG
  PTHREAD_UNSAFE

  if (environ != NULL)
    {
      int x = -1;
      while (environ[++x])
        {
          __os_print ("--execvp: environ["); __os_prdec (x); __os_print ("]: ");
          __os_print (environ[x]); __os_nl ();
        }
    }
#endif

  /* Path searching has not been implemented.  */
  return execve (name, argv, environ);
}
