/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_addr.c,v $
 * $Date: 2000/07/15 14:52:24 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_addr.c,v 1.1.1.1 2000/07/15 14:52:24 nick Exp $";
#endif

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

/* Extract an internet address in network byte order from a string.  */
u_long
inet_addr (const char *cp)
{
  struct in_addr val;

  if (inet_pton (AF_INET, cp, (u_char *) &val.s_addr))
    return val.s_addr;
  return INADDR_NONE;
}
