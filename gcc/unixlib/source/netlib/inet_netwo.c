/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_netwo.c,v $
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
 * Extract a network number in host byte order from a string
 */
in_addr_t
inet_network (const char *cp)
{
  struct in_addr net;

  /* Decode the address string */
  net.s_addr = inet_addr (cp);

  return ntohl (inet_netof (net));
}
