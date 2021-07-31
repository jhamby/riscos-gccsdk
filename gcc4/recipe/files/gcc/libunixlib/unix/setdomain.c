/* setdomainname ()
 * Written by Peter Burwood, July 1997
 * Copyright (c) 1997-2010 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

/* Set the domainname of the host machine.  */
int
setdomainname (const char *name __attribute__ ((unused)),
	       size_t length __attribute__ ((unused)))
{
  /* Bit of a difficult one to set under RISC OS. Set errno
     and return -1 which indicates that the calling process
     is not privileged enough to set the domain name.  */
  return __set_errno (EPERM);
}
