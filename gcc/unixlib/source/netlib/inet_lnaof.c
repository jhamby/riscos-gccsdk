/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_lnaof.c,v $
 * $Date: 2000/07/15 14:52:24 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_lnaof.c,v 1.1.1.1 2000/07/15 14:52:24 nick Exp $";
#endif

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

/*
 * Extract a local network address from an internet address
 */
u_long
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
