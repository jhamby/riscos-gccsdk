/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_mkad.c,v $
 * $Date: 2000/07/15 14:52:25 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_mkad.c,v 1.1.1.1 2000/07/15 14:52:25 nick Exp $";
#endif

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

/*
 * Create an internet address from a network number and a local address
 */
struct in_addr
inet_makeaddr (u_long net, u_long lna)
{
  struct in_addr address;

  address.s_addr = net | lna;

  return address;
}
