/* getservbyport ()
 * Copyright (c) 2010 UnixLib Developers
 */

#include <netdb.h>

#define InetServices_GetServiceByPort 0x55640

struct servent *
getservbyport (int __port, const char *__proto)
{
  /* Given port number is network byte order, the
     InetServices_GetServiceByPort SWI needs host order.  */
  register int port __asm ("r0") = ntohs (__port);
  register int proto __asm ("r1") = (int) __proto;
  struct servent *rtrn;
  __asm volatile ("SWI\t%[SWI_XInetServices_GetServiceByPort]\n\t"
		  "MOVVC\t%[rtrn], r2\n\t"
		  "MOVVS\t%[rtrn], #0\n\t"
		  : [rtrn] "=r" (rtrn)
		  : "r" (port), "r" (proto),
		    [SWI_XInetServices_GetServiceByPort] "i" (InetServices_GetServiceByPort | (1<<17))
		  : "r2", "r14", "cc");
  return rtrn;
}



