/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/setenv.c,v $
 * $Date: 2002/02/14 15:56:39 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setenv.c,v 1.3 2002/02/14 15:56:39 admin Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <swis.h>
#include <pthread.h>

int
setenv (const char *name, const char *value, int replace)
{
  return (__addenv (name, value, replace, strchr (name, '$') != NULL) != NULL
	  ? 0 : -1);
}

/* Remove NAME from the OS environment. If NAME contains a '$', then use NAME
   'as is', otherwise prefix NAME with __UNIX_ENV_PREFIX.  This is not
   symmetric with __getenv_unix_from_os, but it feels wrong to remove NAME
   unconditionally from the global OS environment.  This function is
   symmetric with __addenv_unix_to_os.  */
static void
__remenv_unix_from_os (const char *name)
{
  if (strchr (name, '$') == NULL)
    {
      const size_t len = sizeof (__UNIX_ENV_PREFIX) + strlen (name);
      int rval;
      char *ro_name = malloc (len);

      if (ro_name == NULL)
	return;
      strcpy (ro_name, __UNIX_ENV_PREFIX);
      strcat (ro_name, name);
      rval = __remenv_from_os (ro_name);
      free (ro_name);
      if (rval)
	return;
    }
  else
    __remenv_from_os (name);
}

/* There is a likely memory leak with unsetenv.  If the environment name,value
   pair was malloc'ed by us, then we will leak this memory.  We cannot free
   it in case the memory was not malloc'ed.  */
void
unsetenv (const char *name)
{
  const size_t len = strlen (name);
  char **ep;
  const char *env;

  PTHREAD_UNSAFE

  if (environ != NULL)
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

	  /* Remove it from the OS environment.  */
	  __remenv_unix_from_os (name);
	}
}

/* The `clearenv' was planned to be added to POSIX.1 but probably
   never made it.  Nevertheless the POSIX.9 standard (POSIX bindings
   for Fortran 77) requires this function.  */
/* There is a likely memory leak with clearenv.  If the environment name,value
   pairs were malloc'ed by us, then we will leak that memory.  We cannot free
   it in case the memory was not malloc'ed.  */
int
clearenv (void)
{
  /* We do not remove names from the global OS environment with clearenv.
     If the user wants to remove a global name, they should use unsetenv.  */

  PTHREAD_UNSAFE

  if (environ == __last_environ && environ != NULL)
    {
      /* We allocated this environment so we can free it.  */
      __last_environ = NULL;
      free (environ);
    }

  /* Clear the environment pointer removes the whole environment.  */
  environ = NULL;

  return 0;
}
