/* unsetenv () for UnixLib
   Copyright (c) 2005-2010 UnixLib Developers.  */

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <internal/local.h>
#include <internal/unix.h>

/* There is a likely memory leak with unsetenv.  If the environment name,value
   pair was malloc'ed by us, then we will leak this memory.  We cannot free
   it in case the memory was not malloc'ed.  */
int
unsetenv (const char *name)
{
  PTHREAD_UNSAFE

  if (name == NULL || strchr (name, '='))
    return __set_errno (EINVAL);

  if (strchr (name, '$'))
    return __remenv_from_os (name);

  if (environ != NULL)
    {
      const size_t len = strlen (name);
      char **ep;
      const char *env;

      for (ep = environ; (env = *ep) != NULL; ep++)
	if (!strncmp (env, name, len) && env[len] == '=')
	  {
	    /* Found it.  Remove this pointer by moving later ones back.  */
	    char **dp = ep;
	    do
	      dp[0] = dp[1];
	    while (*dp++);
	    /* Continuing the loop in case NAME appears again, so adjust ep.  */
	    --ep;
	  }
    }

  return 0;
}

