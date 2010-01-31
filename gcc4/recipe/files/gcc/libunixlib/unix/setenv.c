/* Set Unix-style environment variables.
   Copyright (c) 2005, 2007, 2008, 2009 UnixLib Developers.  */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <swis.h>
#include <errno.h>

#include <internal/os.h>
#include <internal/unix.h>
#include <pthread.h>

/* Put value into the OS environment. Will not overwrite an existing
   variable of the same name if replace is 0 */
int
__addenv_to_os (const char *name, const char *value, int replace)
{
  _kernel_oserror *err;
  int regs[10];

  if (!replace)
    {
      /* Check whether the variable already exists */
      regs[0] = (int) name;
      regs[1] = 0;
      regs[2] = -1;
      regs[3] = 0;
      regs[4] = 0;
      __os_swi (OS_ReadVarVal, regs);
      if (regs[2] != 0)
        return 0;
    }

  regs[0] = (int) name;
  regs[1] = (int) value;
  regs[2] = strlen (value);
  regs[3] = 0;
  regs[4] = 4;
  if ((err = __os_swi (OS_SetVarVal, regs)) != NULL)
    return __ul_seterr (err, 1);

  return 0;
}


/* Remove NAME from the OS environment. Returns -1 on failure or not found.  */
static int
__remenv_from_os (const char *name)
{
  _kernel_oserror *err;
  int regs[10];

  regs[0] = (int) name;
  regs[1] = 0;
  regs[2] = -1;
  regs[3] = 0;
  regs[4] = 0;
  /* Do not set the errno if the variable was not found. */
  err = __os_swi (OS_SetVarVal, regs);
  return err ? -1 : 0;
}


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
    __remenv_from_os (name);
  else
    {
      if (environ != NULL)
	{
	  const size_t len = strlen (name);
	  const char **ep;
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
    }

  return 0;
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
  struct ul_global *gbl = &__ul_global;

  /* We do not remove names from the global OS environment with clearenv.
     If the user wants to remove a global name, they should use unsetenv.  */

  PTHREAD_UNSAFE

  if (environ == gbl->last_environ && environ != NULL)
    {
      /* We allocated this environment so we can free it.  */
      gbl->last_environ = NULL;
      free (environ);
    }

  /* Clear the environment pointer removes the whole environment.  */
  environ = NULL;

  return 0;
}
