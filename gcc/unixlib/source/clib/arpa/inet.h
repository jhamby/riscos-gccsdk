/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/arpa/inet.h,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the University of California, Berkeley and its contributors''
 * in the documentation or other materials provided with the distribution
 * and in all advertising materials mentioning features or use of this
 * software. Neither the name of the University nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)inet.h	5.4 (Berkeley) 6/1/90
 */

#ifndef __ARPA_INET_H
#define __ARPA_INET_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

/* Freenet programmers interface - arpa/inet.h - edited by andy 23/5/95 */

#ifndef __NETINET_IN_H
#include <netinet/in.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef __STDDEF_H
#include <stddef.h>
#endif

__BEGIN_DECLS

/*
 * Check whether "cp" is a valid ascii representation
 * of an Internet address and convert to a binary address.
 * Returns 1 if the address is valid, 0 if not.
 * This replaces inet_addr, the return value from which
 * cannot distinguish between failure and a local broadcast address.
 */

extern __u_long inet_aton (const char *cp, struct in_addr *addr);

/*
 * Convert string containing 'dotted' address into numeric
 * internet address
 */
extern __u_long inet_addr (const char *cp);

/*
 * Convert string containing 'dotted' address into numeric
 * internet network number
 */
extern __u_long inet_network (const char *cp);

/*
 * Forms an internet address from a network number and a
 * local network address
 */
extern struct in_addr inet_makeaddr (__u_long net, __u_long lna);

/*
 * Extract the local network address from an internet address
 */
extern __u_long inet_lnaof (struct in_addr in);

/*
 * Extract the network number from an internet address
 */
extern __u_long inet_netof (struct in_addr in);

/*
 * Format an internet address in string form
 */
extern char *inet_ntoa (struct in_addr in);

/*
 * Check whether "src" is a valid ascii representation
 * of an Internet address and convert to a binary address.
 * This replaces inet_addr, the return value from which
 * cannot distinguish between failure and a local broadcast address.
 * return:
 *	1 if the address was valid for the specified address family
 *	0 if the address wasn't valid (`dst' is untouched in this case)
 *	-1 if some other error occurred (`dst' is untouched in this case, too)
 */
extern int inet_pton (int af, const char *src, void *dst);

/*
 * Format an internet address in presentation (ASCII string) form
 */
extern const char *inet_ntop (int af, const void *src, char *dst, size_t s);

__END_DECLS

#endif
