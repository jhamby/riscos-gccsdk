/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/netinet/tcp.h,v $
 * $Date: 2000/07/15 14:52:15 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __NETINET_TCP_H
#define __NETINET_TCP_H

/* Freenet programmers interface - netinet/tcp.h - 23/5/95 */

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef __SYS_BYTEORDER_H
#include <sys/byteorder.h>
#endif

/*
 * Type of a TCP sequence number
 */
typedef __u_long tcp_seq;

/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */

/*
 * Structure of a TCP header
 * Bitfields are necessary for the ARM C compiler in ANSI mode.
 */
struct tcphdr {
  __u_short th_sport;		 /* Source port */
  __u_short th_dport;		 /* Destination port */
  tcp_seq th_seq;	       /* Sequence number */
  tcp_seq th_ack;	       /* Acknowledgement number */
#if BYTE_ORDER == LITTLE_ENDIAN
  __u_int  th_x2  : 4;		/* Unused */
  __u_int  th_off : 4;		/* Data offset */
#endif
#if BYTE_ORDER == BIG_ENDIAN
  __u_int  th_off : 4;		/* Data offset */
  __u_int  th_x2  : 4;		/* Unused */
#endif
  __u_int  th_flags : 8;	    /* Flags */
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
  __u_int   th_win : 16;	 /* Advertised window */
  __u_short th_sum;		 /* Checksum */
  __u_short th_urp;		 /* Urgent pointer */
};


#define	TCPOPT_EOL		0
#define	TCPOPT_NOP		1
#define	TCPOPT_MAXSEG		2
#define TCPOLEN_MAXSEG		4
#define TCPOPT_WINDOW		3
#define TCPOLEN_WINDOW		3
#define TCPOPT_SACK_PERMITTED	4		/* Experimental */
#define TCPOLEN_SACK_PERMITTED	2
#define TCPOPT_SACK		5		/* Experimental */
#define TCPOPT_TIMESTAMP	8
#define TCPOLEN_TIMESTAMP	10
#define TCPOLEN_TSTAMP_APPA	(TCPOLEN_TIMESTAMP+2) /* appendix A */

#define TCPOPT_TSTAMP_HDR	\
    (TCPOPT_NOP<<24|TCPOPT_NOP<<16|TCPOPT_TIMESTAMP<<8|TCPOLEN_TIMESTAMP)

/*
 * Default maximum segment size for TCP.
 * With an IP MSS of 576, this is 536,
 * but 512 is probably more convenient.
 * This should be defined as MIN(512, IP_MSS - sizeof (struct tcpiphdr)).
 */
#define	TCP_MSS	512

#define	TCP_MAXWIN	65535	/* largest value for (unscaled) window */

#define TCP_MAX_WINSHIFT	14	/* maximum window shift */

/*
 * Options for use with [gs]etsockopt at the TCP level. First word of
 * comment is data type; bool is stored in int.
 */
#define TCP_NODELAY 0x01       /* Don't delay send to coalesce packets */
#define TCP_MAXSEG  0x02       /* Set the maximum segment size */

#endif
