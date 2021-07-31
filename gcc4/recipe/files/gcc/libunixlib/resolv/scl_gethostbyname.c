/* gethostbyname ()
 * Copyright (c) 2010 UnixLib Developers
 */

#include <errno.h>
#include <kernel.h>
#include <netdb.h>
#include <stdlib.h>

#include <internal/os.h>

#define Resolver_GetHostByName 0x46000

struct hostent *
gethostbyname (const char *__name)
{
  register const char *name __asm ("r1") = __name;
  const _kernel_oserror *roerr;
  register struct hostent *rtrn __asm ("r1");
  __asm volatile ("SWI\t%[SWI_XResolver_GetHostByName]\n\t"
		  "MOVVS\t%[roerr], r0\n\t"
		  "MOVVC\t%[roerr], #0\n\t"
		  : [roerr] "=r" (roerr)
		  : "r" (name),
		    [SWI_XResolver_GetHostByName] "i" (Resolver_GetHostByName | (1<<17))
		  : "r0", "r14", "cc");

  int ulerrno;
  if (roerr != NULL)
    {
      /* RISC OS error happened.  */
      __ul_seterr (roerr, EOPSYS);
      ulerrno = NETDB_INTERNAL;
    }
  else if (ulerrno == -1) /* Host not found.  */
    ulerrno = HOST_NOT_FOUND;
  else if (ulerrno != 0)
    ulerrno = TRY_AGAIN;
  else
    ulerrno = NETDB_SUCCESS;

  if (ulerrno != NETDB_SUCCESS)
    __set_h_errno (ulerrno);

  return rtrn;
}
