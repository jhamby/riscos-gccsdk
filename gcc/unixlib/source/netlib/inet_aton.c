/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_aton.c,v $
 * $Date: 2000/07/15 14:52:24 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_aton.c,v 1.1.1.1 2000/07/15 14:52:24 nick Exp $";
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
