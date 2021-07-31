/* inet_netof ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

/*
 * Extract a network number from an internet address
 */
in_addr_t
inet_netof (struct in_addr in)
{
  u_long net;

  if (IN_CLASSA (in.s_addr))
    {
      /* Extract a class A network number */
      net = in.s_addr & IN_CLASSA_NET;
    }
  else if (IN_CLASSB (in.s_addr))
    {
      /* Extract a class B network number */
      net = in.s_addr & IN_CLASSB_NET;
    }
  else if (IN_CLASSC (in.s_addr))
    {
      /* Extract a class C network number */
      net = in.s_addr & IN_CLASSC_NET;
    }
  else
    {
      /* Not a known address format */
      net = 0;
    }

  return net;
}
