/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/inet_netof,v $
 * $Date: 1997/10/09 20:00:19 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_netof,v 1.5 1997/10/09 20:00:19 unixlib Exp $";
#endif

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

/*
 * Extract a network number from an internet address
 */
u_long
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
