/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_netwo.c,v $
 * $Date: 2000/07/15 14:52:25 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_netwo.c,v 1.1.1.1 2000/07/15 14:52:25 nick Exp $";
#endif

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/byteorder.h>
#include <sys/types.h>

/*
 * Extract a network number in host byte order from a string
 */
u_long
inet_network (const char *cp)
{
  struct in_addr net;

  /* Decode the address string */
  net.s_addr = inet_addr (cp);

  return ntohl (inet_netof (net));
}
