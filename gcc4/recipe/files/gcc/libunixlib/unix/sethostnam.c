/* Set internet hostname for machine.
   Copyright (c) 2003-2010 UnixLib Developers.  */

#include <errno.h>
#include <unistd.h>

/* Set the name of the host machine.  */
int
sethostname (const char *name __attribute__ ((unused)),
	     size_t length __attribute__ ((unused)))
{
  /* Bit of a difficult one to set under RISC OS. Set errno
     and return -1 which indicates that the calling process
     is not privileged enough to set the host name.  */
  return __set_errno (EPERM);
}
