/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/netinet/udp.h,v $
 * $Date: 2000/07/15 14:52:15 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
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
