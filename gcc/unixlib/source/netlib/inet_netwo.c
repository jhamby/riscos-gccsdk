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
