/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_netof.c,v $
 * $Date: 2000/07/15 14:52:25 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_netof.c,v 1.1.1.1 2000/07/15 14:52:25 nick Exp $";
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
