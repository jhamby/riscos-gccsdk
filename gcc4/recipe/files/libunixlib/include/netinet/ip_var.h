/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __NETINET_IP_VAR_H
#define __NETINET_IP_VAR_H 1

/* Freenet programmers interface - netinet/ip_var.h - 23/5/95 */
#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef _NETINET_IN_H
#include <netinet/in.h>
#endif

__BEGIN_DECLS

/*
 * Overlay for ip header used by other protocols (tcp, udp).
 */
struct ipovly {
  __caddr_t	ih_next, ih_prev;	/* for protocol sequence q's */
  __u_char	ih_x1;			/* (unused) */
  __u_char	ih_pr;			/* protocol */
  short		ih_len;			/* protocol length */
  struct	in_addr ih_src;		/* source internet address */
  struct	in_addr ih_dst;		/* destination internet address */
};

/*
 * Ip reassembly queue structure.  Each fragment
 * being reassembled is attached to one of these structures.
 * They are timed out after ipq_ttl drops to 0, and may also
 * be reclaimed if memory becomes tight.
 */
struct ipq {
  struct	ipq *next,*prev;	/* to other reass headers */
  __u_char	ipq_ttl;		/* time for reass q to live */
  __u_char	ipq_p;			/* protocol of this fragment */
  __u_short	ipq_id;			/* sequence id for reassembly */
  struct	ipasfrag *ipq_next,*ipq_prev;
					/* to ip headers of fragments */
  struct	in_addr ipq_src,ipq_dst;
};

/*
 * Ip header, when holding a fragment.
 *
 * Note: ipf_next must be at same offset as ipq_next above
 * Bitfields are necessary for the ARM C compiler in ANSI mode.
 */
struct	ipasfrag {
#if BYTE_ORDER == LITTLE_ENDIAN
  __u_int	ip_hl : 4;
  __u_int	ip_v  : 4;
#endif
#if BYTE_ORDER == BIG_ENDIAN
  __u_int	ip_v  : 4;
  __u_int	ip_hl : 4;
#endif
  __u_int	ipf_mff : 8;		/* XXX overlays ip_tos: use low bit
					 * to avoid destroying tos;
					 * copied from (ip_off&IP_MF) */
  __u_int	ip_len : 16;
  __u_short	ip_id;
  short		ip_off;
  __u_char	ip_ttl;
  __u_char	ip_p;
  __u_short	ip_sum;
  struct	ipasfrag *ipf_next;	/* next fragment */
  struct	ipasfrag *ipf_prev;	/* previous fragment */
};

/*
 * Structure stored in mbuf in inpcb.ip_options
 * and passed to ip_output when ip options are in use.
 * The actual length of the options (including ipopt_dst)
 * is in m_len.
 */
#define MAX_IPOPTLEN	40

struct ipoption {
	struct	in_addr ipopt_dst;	/* first-hop dst if source routed */
	char	ipopt_list[MAX_IPOPTLEN];	/* options proper */
};

/*
 * Structure attached to inpcb.ip_moptions and
 * passed to ip_output when IP multicast options are in use.
 */
struct ip_moptions {
  struct	ifnet *imo_multicast_ifp; /* ifp for outgoing multicasts */
  __u_char	imo_multicast_ttl;	/* TTL for outgoing multicasts */
  __u_char	imo_multicast_loop;	/* 1 => hear sends if a member */
  __u_short	imo_num_memberships;	/* no. memberships this socket */
  struct	in_multi *imo_membership[IP_MAX_MEMBERSHIPS];
};

struct	ipstat {
  __u_long	ips_total;		/* total packets received */
  __u_long	ips_badsum;		/* checksum bad */
  __u_long	ips_tooshort;		/* packet too short */
  __u_long	ips_toosmall;		/* not enough data */
  __u_long	ips_badhlen;		/* ip header length < data size */
  __u_long	ips_badlen;		/* ip length < ip header length */
  __u_long	ips_fragments;		/* fragments received */
  __u_long	ips_fragdropped;	/* frags dropped (dups, out of space) */
  __u_long	ips_fragtimeout;	/* fragments timed out */
  __u_long	ips_forward;		/* packets forwarded */
  __u_long	ips_cantforward;	/* packets rcvd for unreachable dest */
  __u_long	ips_redirectsent;	/* packets forwarded on same net */
  __u_long	ips_noproto;		/* unknown or unsupported protocol */
  __u_long	ips_delivered;		/* datagrams delivered to upper level*/
  __u_long	ips_localout;		/* total ip packets generated here */
  __u_long	ips_odropped;		/* lost packets due to nobufs, etc. */
  __u_long	ips_reassembled;	/* total packets reassembled ok */
  __u_long	ips_fragmented;		/* datagrams sucessfully fragmented */
  __u_long	ips_ofragments;		/* output fragments created */
  __u_long	ips_cantfrag;		/* don't fragment flag was set, etc. */
  __u_long	ips_badoptions;		/* error in option processing */
  __u_long	ips_noroute;		/* packets discarded due to no route */
  __u_long	ips_badvers;		/* ip version != 4 */
  __u_long	ips_rawout;		/* total raw ip packets generated */
};

/*
 * Maximum size of the IP options for a packet
 */
#define	MAX_IPOPTLEN 40

__END_DECLS

#endif
