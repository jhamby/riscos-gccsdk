/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/netinet/ip_icmp.h,v $
 * $Date: 2000/07/15 14:52:15 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __NETINET_IP_ICMP_H
#define __NETINET_IP_ICMP_H

/* Freenet programmers interface - netinet/ip_icmp.h - 23/5/95 */


#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef __NETINET_IP_H
#include <netinet/ip.h>
#endif

#ifndef __NETINET_IN_H
#include <netinet/in.h>
#endif

#ifndef __NETINET_IN_SYSTM_H
#include <netinet/in_systm.h>
#endif

/*
 * Interface Control Message Protocol Definitions.
 * Per RFC 792, September 1981.
 */

/*
 * Structure of an icmp header.
 */
struct icmp {
	__u_char	icmp_type;		/* type of message, see below */
	__u_char	icmp_code;		/* type sub code */
	__u_short	icmp_cksum;		/* ones complement cksum of struct */
	union {
		__u_char ih_pptr;			/* ICMP_PARAMPROB */
		struct in_addr ih_gwaddr;	/* ICMP_REDIRECT */
		struct ih_idseq {
			n_short	icd_id;
			n_short	icd_seq;
		} ih_idseq;
		int ih_void;

		/* ICMP_UNREACH_NEEDFRAG -- Path MTU Discovery (RFC1191) */
		struct ih_pmtu {
			n_short ipm_void;
			n_short ipm_nextmtu;
		} ih_pmtu;
	} icmp_hun;
#define	icmp_pptr	icmp_hun.ih_pptr
#define	icmp_gwaddr	icmp_hun.ih_gwaddr
#define	icmp_id		icmp_hun.ih_idseq.icd_id
#define	icmp_seq	icmp_hun.ih_idseq.icd_seq
#define	icmp_void	icmp_hun.ih_void
#define	icmp_pmvoid	icmp_hun.ih_pmtu.ipm_void
#define	icmp_nextmtu	icmp_hun.ih_pmtu.ipm_nextmtu
	union {
		struct id_ts {
			n_time its_otime;
			n_time its_rtime;
			n_time its_ttime;
		} id_ts;
		struct id_ip  {
			struct ip idi_ip;
			/* options and then 64 bits of data */
		} id_ip;
		__u_long	id_mask;
		char	id_data[1];
	} icmp_dun;
#define	icmp_otime	icmp_dun.id_ts.its_otime
#define	icmp_rtime	icmp_dun.id_ts.its_rtime
#define	icmp_ttime	icmp_dun.id_ts.its_ttime
#define	icmp_ip		icmp_dun.id_ip.idi_ip
#define	icmp_mask	icmp_dun.id_mask
#define	icmp_data	icmp_dun.id_data
};

/*
 * Minimum ICMP packet size
 */
#define	ICMP_MINLEN	8
#define	ICMP_TSLEN	(8 + 3 * sizeof (n_time))	/* timestamp */
#define	ICMP_MASKLEN	12				/* address mask */
#define	ICMP_ADVLENMIN	(8 + sizeof (struct ip) + 8)	/* min */
#define	ICMP_ADVLEN(p)	(8 + ((p)->icmp_ip.ip_hl << 2) + 8)

/*
 * Values for type and code fields
 */
#define ICMP_ECHOREPLY    0        /* Echo reply */
#define ICMP_UNREACH      3        /* Destination unreachable */
#define    ICMP_UNREACH_NET      0    /* Bad net */
#define    ICMP_UNREACH_HOST     1    /* Bad host */
#define    ICMP_UNREACH_PROTOCOL 2    /* Bad protocol */
#define    ICMP_UNREACH_PORT     3    /* Bad port */
#define    ICMP_UNREACH_NEEDFRAG 4    /* Fragmentation needed */
#define    ICMP_UNREACH_SRCFAIL  5    /* Source route failed */
#define	ICMP_UNREACH_NET_UNKNOWN 6	/* unknown net */
#define	ICMP_UNREACH_HOST_UNKNOWN 7	/* unknown host */
#define	ICMP_UNREACH_ISOLATED	8	/* src host isolated */
#define	ICMP_UNREACH_NET_PROHIB	9	/* prohibited access */
#define	ICMP_UNREACH_HOST_PROHIB 10	/* ditto */
#define	ICMP_UNREACH_TOSNET	11	/* bad tos for net */
#define	ICMP_UNREACH_TOSHOST	12	/* bad tos for host */
#define ICMP_SOURCEQUENCH 4        /* Source quench */
#define ICMP_REDIRECT     5        /* Redirection advice */
#define    ICMP_REDIRECT_NET     0    /* For network */
#define    ICMP_REDIRECT_HOST    1    /* For host */
#define    ICMP_REDIRECT_TOSNET  2    /* For network and TOS */
#define    ICMP_REDIRECT_TOSHOST 3    /* For host and TOS */
#define ICMP_ECHO         8        /* Echo request */
#define	ICMP_ROUTERADVERT	9	/* router advertisement */
#define	ICMP_ROUTERSOLICIT	10	/* router solicitation */
#define ICMP_TIMXCEED     11       /* Time exceeded */
#define    ICMP_TIMXCEED_INTRANS 0    /* TTL exceeded */
#define    ICMP_TIMXCEED_REASS   1    /* Reassembly timed out */
#define ICMP_PARAMPROB    12       /* Parameter problem */
#define	ICMP_PARAMPROB_OPTABSENT 1	/* req. opt. absent */
#define ICMP_TSTAMP       13       /* Timestamp request */
#define ICMP_TSTAMPREPLY  14       /* Timestamp reply */
#define ICMP_IREQ         15       /* Information request */
#define ICMP_IREQREPLY    16       /* Information reply */
#define ICMP_MASKREQ      17       /* Address mask request */
#define ICMP_MASKREPLY    18       /* Address mask reply */

#define	ICMP_MAXTYPE		18

#define	ICMP_INFOTYPE(type) \
	((type) == ICMP_ECHOREPLY || (type) == ICMP_ECHO || \
	(type) == ICMP_ROUTERADVERT || (type) == ICMP_ROUTERSOLICIT || \
	(type) == ICMP_TSTAMP || (type) == ICMP_TSTAMPREPLY || \
	(type) == ICMP_IREQ || (type) == ICMP_IREQREPLY || \
	(type) == ICMP_MASKREQ || (type) == ICMP_MASKREPLY)

#endif
