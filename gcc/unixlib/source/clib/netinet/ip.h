/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/netinet/ip.h,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __NETINET_IP_H
#define __NETINET_IP_H

/* Freenet programmers interface - netinet/ip.h - 23/5/95 */

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef __NETINET_IN_H
#include <netinet/in.h>
#endif

#ifndef __NETINET_IN_SYSTM_H
#include <netinet/in_systm.h>
#endif

#ifndef __SYS_BYTEORDER_H
#include <sys/byteorder.h>
#endif

__BEGIN_DECLS

/*
 * Current IP version
 */
#define IPVERSION 4

/*
 * Structure of an IP header
 * Bitfields are necessary for the ARM C compiler in ANSI mode.
 */
struct ip
{
#if BYTE_ORDER == LITTLE_ENDIAN
  __u_int	 ip_hl  : 4;  /* Header length */
  __u_int	 ip_v	: 4;  /* Protocol version */
#endif
#if BYTE_ORDER == BIG_ENDIAN
  __u_int	 ip_v	: 4;  /* Protocol version */
  __u_int	 ip_hl  : 4;  /* Header length */
#endif
  __u_int	 ip_tos : 8;  /* Type of service */
  __u_int	 ip_len : 16; /* Packet length  - ANSI wants int not short */
  __u_short	 ip_id;	      /* Packet ID */
  short		 ip_off;      /* Fragment offset */
#define IP_DF 0x4000	      /* Don't fragment flag */
#define IP_MF 0x2000	      /* More fragments flag */
#define	IP_OFFMASK 0x1fff     /* mask for fragmenting bits */
  __u_char	 ip_ttl;      /* Time-to-live */
  __u_char	 ip_p;	      /* Protocol */
  __u_short	 ip_sum;      /* Checksum */
  struct in_addr ip_src;      /* Source address */
  struct in_addr ip_dst;      /* Destination address */
};

/*
 * Maximum size of an IP packet
 */
#define	IP_MAXPACKET	65535

/*
 * Definitions for IP type of service (ip_tos)
 */
#define	IPTOS_LOWDELAY		0x10
#define	IPTOS_THROUGHPUT	0x08
#define	IPTOS_RELIABILITY	0x04

/*
 * Definitions for IP precedence (also in ip_tos) (hopefully unused)
 */
#define	IPTOS_PREC_NETCONTROL		0xe0
#define	IPTOS_PREC_INTERNETCONTROL	0xc0
#define	IPTOS_PREC_CRITIC_ECP		0xa0
#define	IPTOS_PREC_FLASHOVERRIDE	0x80
#define	IPTOS_PREC_FLASH		0x60
#define	IPTOS_PREC_IMMEDIATE		0x40
#define	IPTOS_PREC_PRIORITY		0x20
#define	IPTOS_PREC_ROUTINE		0x10

/*
 * Option codes for IP options
 */
#define	IPOPT_COPIED(o)		((o)&0x80)
#define	IPOPT_CLASS(o)		((o)&0x60)
#define	IPOPT_NUMBER(o)		((o)&0x1f)

#define	IPOPT_CONTROL		0x00
#define	IPOPT_RESERVED1		0x20
#define	IPOPT_DEBMEAS		0x40
#define	IPOPT_RESERVED2		0x60

#define	IPOPT_EOL		0		/* End of option list */
#define	IPOPT_NOP		1		/* No operation */
#define	IPOPT_RR		7		/* Record packet route */
#define	IPOPT_TS		68		/* Timestamp */
#define	IPOPT_SECURITY		130		/* Provide s,c,h,tcc */
#define	IPOPT_LSRR		131		/* Loose source route */
#define	IPOPT_SATID		136		/* Satnet id */
#define	IPOPT_SSRR		137		/* Strict source route */

/*
 * Offsets into IP options
 */
#define	IPOPT_OPTVAL		0		/* Option ID */
#define	IPOPT_OLEN		1		/* Option length */
#define	IPOPT_OFFSET		2		/* Offset within option */
#define	IPOPT_MINOFF		4		/* Min value of above */

/*
 * Time stamp option structure.
 * Bitfields are necessary for the ARM C compiler in ANSI mode.
 */
struct	ip_timestamp
{
  __u_int	ipt_code : 8;		/* IPOPT_TS */
  __u_int	ipt_len  : 8;		/* size of structure (variable) */
  __u_int	ipt_ptr  : 8;		/* index of current entry */
#if BYTE_ORDER == LITTLE_ENDIAN
  __u_int	ipt_flg  : 4;		/* flags, see below */
  __u_int	ipt_oflw : 4;		/* overflow counter */
#endif
#if BYTE_ORDER == BIG_ENDIAN
  __u_int	ipt_oflw : 4;		/* overflow counter */
  __u_int	ipt_flg  : 4;		/* flags, see below */
#endif
  union ipt_timestamp
  {
    n_long	ipt_time[1];
    struct	ipt_ta
    {
      struct in_addr ipt_addr;
      n_long ipt_time;
    } ipt_ta[1];
  } ipt_timestamp;
};

/* flag bits for ipt_flg */
#define	IPOPT_TS_TSONLY		0		/* timestamps only */
#define	IPOPT_TS_TSANDADDR	1		/* timestamps and addresses */
#define	IPOPT_TS_PRESPEC	3		/* specified modules only */

/* bits for security (not byte swapped) */
#define	IPOPT_SECUR_UNCLASS	0x0000
#define	IPOPT_SECUR_CONFID	0xf135
#define	IPOPT_SECUR_EFTO	0x789a
#define	IPOPT_SECUR_MMMM	0xbc4d
#define	IPOPT_SECUR_RESTR	0xaf13
#define	IPOPT_SECUR_SECRET	0xd788
#define	IPOPT_SECUR_TOPSECRET	0x6bc5

/*
 * Standard values for various parameters
 */
#define	MAXTTL		255		/* Maximum time to live (seconds) */
#define	IPDEFTTL	64		/* Default ttl, from RFC 1340 */
#define	IPFRAGTTL	60		/* Time to live for frags, slowhz */
#define	IPTTLDEC	1		/* Subtracted when forwarding */
#define	IP_MSS		576		/* Default maximum segment size */

__END_DECLS

#endif
