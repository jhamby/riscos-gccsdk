/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

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
