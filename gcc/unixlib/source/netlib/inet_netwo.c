/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/inet_netwo,v $
 * $Date: 1997/10/09 20:00:19 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_netwo,v 1.5 1997/10/09 20:00:19 unixlib Exp $";
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
