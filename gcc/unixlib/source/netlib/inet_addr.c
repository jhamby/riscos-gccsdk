/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/inet_addr,v $
 * $Date: 1997/12/17 22:27:07 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_addr,v 1.6 1997/12/17 22:27:07 unixlib Exp $";
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
