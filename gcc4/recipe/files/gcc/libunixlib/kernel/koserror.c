/* _kernel_oserror()
 * Copyright (c) 2010 UnixLib Developers
 */

#include <kernel.h>
#include <pthread.h>

/* Returns the last RISC OS error happened for the current thread, or NULL
   when non did happen.  */
_kernel_oserror *
_kernel_last_oserror (void)
{
  _kernel_oserror *err;
  if (__pthread_running_thread->errbuf_valid)
    {
      /* Calling _kernel_last_oserror makes it invalidating the RISC OS
	 error buffer.  */
      __pthread_running_thread->errbuf_valid = '\0';
      err = &__pthread_running_thread->errbuf;
    }
  else
    err = NULL;

  return err;
}

