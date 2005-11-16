/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_mkad.c,v $
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
 * Create an internet address from a network number and a local address
 */
struct in_addr
inet_makeaddr (in_addr_t net, in_addr_t lna)
{
  struct in_addr address;

  address.s_addr = net | lna;

  return address;
}
