/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/inet_mkad,v $
 * $Date: 1997/10/09 20:00:19 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_mkad,v 1.5 1997/10/09 20:00:19 unixlib Exp $";
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
