/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/netinet/udp.h,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __NETINET_UDP_H
#define __NETINET_UDP_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

__BEGIN_DECLS

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

__END_DECLS

#endif
