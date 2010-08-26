/* clearenv () for UnixLib
   Copyright (c) 2005-2010 UnixLib Developers.  */

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <internal/local.h>
#include <internal/unix.h>

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

  struct ul_global *gbl = &__ul_global;
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
