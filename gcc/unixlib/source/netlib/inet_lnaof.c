/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/inet_lnaof,v $
 * $Date: 1997/10/09 20:00:19 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_lnaof,v 1.5 1997/10/09 20:00:19 unixlib Exp $";
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
