/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/inet_aton,v $
 * $Date: 1997/12/17 22:27:07 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_aton,v 1.6 1997/12/17 22:27:07 unixlib Exp $";
#endif

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

/* Check whether CP is a valid ascii representation
   of an Internet address and convert to a binary address.
   Returns 1 if the address is valid, 0 if not.
   This replaces inet_addr, the return value from which
   cannot distinguish between failure and a local broadcast address.  */

u_long
inet_aton (const char *cp, struct in_addr *addr)
{
  return inet_pton (AF_INET, cp, (u_char *) &addr->s_addr);
}
