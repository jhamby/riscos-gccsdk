/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_lnaof.c,v $
 * $Date: 2002/12/22 18:22:29 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

/*
 * Extract a local network address from an internet address
 */
in_addr_t
inet_lnaof (struct in_addr in)
{
  u_long lna;

  if (IN_CLASSA (in.s_addr))
    {
      /* Extract a class A local address */
      lna = in.s_addr & IN_CLASSA_HOST;
    }
  else if (IN_CLASSB (in.s_addr))
    {
      /* Extract a class B local address */
      lna = in.s_addr & IN_CLASSB_HOST;
    }
  else if (IN_CLASSC (in.s_addr))
    {
      /* Extract a class C local address */
      lna = in.s_addr & IN_CLASSC_HOST;
    }
  else
    {
      /* Not a known address format */
      lna = in.s_addr;
    }

  return lna;
}
