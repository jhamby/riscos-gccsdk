/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/netinet/h/in,v $
 * $Date: 1997/12/17 22:27:06 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

/*
 * Copyright (c) 1982, 1986, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)in.h	8.3 (Berkeley) 1/3/94
 */

#ifndef __NETINET_IN_H
#define __NETINET_IN_H 1

/* Freenet programmers interface - netinet/in.h - edited by andy 23/5/95 */

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

/* Constants and structures defined by the internet system,
   Per RFC 790, September 1981, and numerous additions.  */


/* Standard well-defined IP protocols.  */
enum
  {
    IPPROTO_IP	 = 0,		/* Dummy protocol for TCP.  */
    IPPROTO_ICMP = 1,		/* Internet Control Message Protocol.  */
    IPPROTO_IGMP = 2,		/* Internet Group Management Protocol.  */
    IPPROTO_GGP  = 3,		/* Gateway^2 (deprecated).  */
    IPPROTO_IPIP = 4,		/* IP inside IP.  */
    IPPROTO_TCP  = 6,		/* Transmission Control Protocol.  */
    IPPROTO_EGP  = 8,		/* Exterior Gateway Protocol.  */
    IPPROTO_PUP  = 12,		/* PUP protocol.  */
    IPPROTO_UDP  = 17,		/* User Datagram Protocol.  */
    IPPROTO_IDP  = 22,		/* XNS IDP protocol.  */
    IPPROTO_TP	 = 29,		/* TP-4 with class negotiation.  */
    IPPROTO_EON  = 80,		/* ISO cnlp.  */
    IPPROTO_ENCAP = 98,		/* Encapsulation header.  */

    IPPROTO_RAW = 255,		/* Raw IP packets.  */
    IPPROTO_MAX
  };

/* Standard well-known ports.  */
enum
  {
    IPPORT_ECHO = 7,		/* Echo service.  */
    IPPORT_DISCARD = 9,		/* Discard transmissions service.  */
    IPPORT_SYSTAT = 11,		/* System status service.  */
    IPPORT_DAYTIME = 13,	/* Time of day service.  */
    IPPORT_NETSTAT = 15,	/* Network status service.  */
    IPPORT_FTP = 21,		/* File Transfer Protocol.  */
    IPPORT_TELNET = 23,		/* Telnet protocol.  */
    IPPORT_SMTP = 25,		/* Simple Mail Transfer Protocol.  */
    IPPORT_TIMESERVER = 37,	/* Timeserver service.  */
    IPPORT_NAMESERVER = 42,	/* Domain Name Service.  */
    IPPORT_WHOIS = 43,		/* Internet Whois service.  */
    IPPORT_MTP = 57,

    IPPORT_TFTP = 69,		/* Trivial File Transfer Protocol.  */
    IPPORT_RJE = 77,
    IPPORT_FINGER = 79,		/* Finger service.  */
    IPPORT_TTYLINK = 87,
    IPPORT_SUPDUP = 95,		/* SUPDUP protocol.  */


    IPPORT_EXECSERVER = 512,	/* execd service.  */
    IPPORT_LOGINSERVER = 513,	/* rlogind service.  */
    IPPORT_CMDSERVER = 514,
    IPPORT_EFSSERVER = 520,

    /* UDP ports.  */
    IPPORT_BIFFUDP = 512,
    IPPORT_WHOSERVER = 513,
    IPPORT_ROUTESERVER = 520,

    /* Ports less than this value are reserved for privileged processes.  */
    IPPORT_RESERVED = 1024,

    /* Ports greater this value are reserved for (non-privileged) servers.  */
    IPPORT_USERRESERVED = 5000
  };

/* Internet address (a structure for historical reasons).  */
struct in_addr
{
  __u_long s_addr;
};

struct in6_addr
{
  __u_char s6_addr[16];		/* IPv6 address.  */
};

/* Definitions of the bits in an Internet address integer.

   On subnets, host and network parts are found according to
   the subnet mask, not these masks.  */

/* Macros for decomposing class A addresses.  */
#define	IN_CLASSA(i)	    (((long)(i) & 0x80000000) == 0)
#define	IN_CLASSA_NET	    0xff000000
#define	IN_CLASSA_NSHIFT    24
#define	IN_CLASSA_HOST	    0x00ffffff
#define	IN_CLASSA_MAX	    128

/* Macros for decomposing class B addresses.  */
#define	IN_CLASSB(i)	    (((long)(i) & 0xc0000000) == 0x80000000)
#define	IN_CLASSB_NET	    0xffff0000
#define	IN_CLASSB_NSHIFT    16
#define	IN_CLASSB_HOST	    0x0000ffff
#define	IN_CLASSB_MAX	    65536

/* Macros for decomposing class C addresses.  */
#define	IN_CLASSC(i)	    (((long)(i) & 0xe0000000) == 0xc0000000)
#define	IN_CLASSC_NET	    0xffffff00
#define	IN_CLASSC_NSHIFT    8
#define	IN_CLASSC_HOST	    0x000000ff

/* Macros for decomposing class D addresses.  */
#define	IN_CLASSD(i)		(((long)(i) & 0xf0000000) == 0xe0000000)
/* These ones aren't really net and host fields, but routing needn't know.  */
#define	IN_CLASSD_NET		0xf0000000
#define	IN_CLASSD_NSHIFT	28
#define	IN_CLASSD_HOST		0x0fffffff
#define	IN_MULTICAST(i)		IN_CLASSD(i)

#define	IN_EXPERIMENTAL(i)	(((long)(i) & 0xf0000000) == 0xf0000000)
#define	IN_BADCLASS(i)		(((long)(i) & 0xf0000000) == 0xf0000000)

/*
 * Some useful addresses
 */
/* Address to accept any incoming messages.  */
#define INADDR_ANY	    ((__u_long)0x00000000)
/* Address to loopback in software to local host.  */
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK	    ((__u_long)0x7f000001)	/* 127.0.0.1.  */
#endif

/* Address to send to all hosts.  */
#define INADDR_BROADCAST    ((__u_long)0xffffffff)
/* Address indicating an error return.  */
#define INADDR_NONE	    0xffffffff

#define	INADDR_UNSPEC_GROUP	(__u_long)0xe0000000	/* 224.0.0.0 */
#define	INADDR_ALLHOSTS_GROUP	(__u_long)0xe0000001	/* 224.0.0.1 */
#define	INADDR_MAX_LOCAL_GROUP	(__u_long)0xe00000ff	/* 224.0.0.255 */

/* Network number for local host loopback.  */
#define	IN_LOOPBACKNET		127

/* Definition of an internet style socket address.  */
struct sockaddr_in
{
  short		 sin_family;	 /* Family (AF_INET).  */
  __u_short	 sin_port;	 /* Port number.  */
  struct in_addr sin_addr;	 /* Internet address.  */
  char		 sin_zero[8];	 /* Padding.  */
};

struct sockaddr_in6
{
  __u_short		sin6_family;	/* AF_INET6.  */
  __u_short		sin6_port;	/* Transport layer port number.  */
  __u_long		sin6_flowinfo;	/* IPv6 flow information.  */
  struct in6_addr	sin6_addr;	/* IPv6 address.  */
};

/* Options for use with `getsockopt' and `setsockopt' at the IP level.
   The first word in the comment at the right is the data type used;
   "bool" means a boolean value stored in an `int'.  */
#define	IP_OPTIONS	      1	   /* buf/ip_opts; set/get IP options.  */
#define	IP_HDRINCL	      2	   /* int; header is included with data.  */
#define	IP_TOS		      3	   /* int; IP type of service and preced.  */
#define	IP_TTL		      4	   /* int; IP time to live.  */
#define	IP_RECVOPTS	      5	   /* bool; receive all IP opts w/dgram.  */
#define	IP_RECVRETOPTS	      6	   /* bool; receive IP opts for response.  */
#define	IP_RECVDSTADDR	      7	   /* bool; receive IP dst addr w/dgram.  */
#define	IP_RETOPTS	      8	   /* ip_opts; set/get IP options.  */
#define	IP_MULTICAST_IF	      9	   /* u_char; set/get IP multicast i/f.  */
#define	IP_MULTICAST_TTL      10   /* u_char; set/get IP multicast ttl. */
#define	IP_MULTICAST_LOOP     11   /* u_char; set/get IP multicast loopback.  */
#define	IP_ADD_MEMBERSHIP     12   /* ip_mreq; add an IP group membership.  */
#define	IP_DROP_MEMBERSHIP    13   /* ip_mreq; drop an IP group membership.  */

/* Defaults and limits for options.  */
#define	IP_DEFAULT_MULTICAST_TTL  1	/* normally limit m'casts to 1 hop  */
#define	IP_DEFAULT_MULTICAST_LOOP 1	/* normally hear sends if a member  */
#define	IP_MAX_MEMBERSHIPS	20	/* per socket; must fit in one mbuf */

/* Structure used to describe IP options for IP_OPTIONS and IP_RETOPTS.
   The `ip_dst' field is used for the first-hop gateway when using a
   source route (this gets put into the header proper).  */
struct ip_opts
{
  struct in_addr ip_dst;	/* First hop; zero without source route.  */
  char ip_opts[40];		/* Actually variable in size.  */
};

/* Argument structure for IP_ADD_MEMBERSHIP and IP_DROP_MEMBERSHIP.  */
struct ip_mreq
{
  struct in_addr imr_multiaddr;	/* IP multicast address of group.  */
  struct in_addr imr_interface;	/* Local IP address of interface.  */
};

/* The host byte order and network byte order translation functions are
   normally declared in this .h file, so pull them in.  */
#include <sys/byteorder.h>

#endif
