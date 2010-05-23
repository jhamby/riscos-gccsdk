/* getservbyname ()
 * Copyright (c) 2010 UnixLib Developers
 */

#include <netdb.h>

#define InetServices_GetServiceByName 0x55641

struct servent *
getservbyname (const char *__name, const char *__proto)
{
  register const char *name __asm ("r0") = __name;
  register const char *proto __asm ("r1") = __proto;
  struct servent *rtrn;
  __asm volatile ("SWI\t%[SWI_XInetServices_GetServiceByName]\n\t"
		  "MOVVC\t%[rtrn], r2\n\t"
		  "MOVVS\t%[rtrn], #0\n\t"
		  : [rtrn] "=r" (rtrn)
		  : "r" (name), "r" (proto),
		    [SWI_XInetServices_GetServiceByName] "i" (InetServices_GetServiceByName | (1<<17))
		  : "r2", "r14", "cc");
  return rtrn;
}

