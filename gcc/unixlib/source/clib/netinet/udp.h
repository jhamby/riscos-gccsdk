/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/netinet/h/udp,v $
 * $Date: 1997/10/09 19:59:52 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __NETINET_UDP_H
#define __NETINET_UDP_H

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

/*
 * Structure of a UDP header
 * Per RFC 768, September, 1981.
 */

struct udphdr
{
  __u_short uh_sport;      /* Source port */
  __u_short uh_dport;      /* Destination port */
  short uh_ulen;       	   /* Length */
  __u_short uh_sum;        /* Checksum */
};

#endif
