/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/socket.h,v $
 * $Date: 2000/07/15 14:52:16 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/*
 * Copyright (c) 1982, 1985, 1986, 1988, 1993, 1994
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
 *	@(#)socket.h	8.4 (Berkeley) 2/21/94
 */

#ifndef __SYS_SOCKET_H
#define __SYS_SOCKET_H

/* Freenet programmers interface - sys/socket.h - edit by andy 23/5/95 */

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef __SYS_UIO_H
#include <sys/uio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Definitions related to sockets: types, address families, options.
 */

/*
 * Socket types
 */
#define SOCK_STREAM	1	  /* Stream socket */
#define SOCK_DGRAM	2	  /* Datagram socket */
#define SOCK_RAW	3	  /* Raw socket */
#define	SOCK_RDM	4	  /* reliably-delivered message */
#define	SOCK_SEQPACKET	5	  /* sequenced packet stream */

/*
 * Address families
 */
#define	AF_UNSPEC	0		/* unspecified */
#define	AF_LOCAL	1		/* local to host (pipes, portals) */
#define	AF_UNIX		AF_LOCAL	/* backward compatibility */
#define AF_INET	    2	      /* Internet */
#define	AF_IMPLINK	3		/* arpanet imp addresses */
#define	AF_PUP		4		/* pup protocols: e.g. BSP */
#define	AF_CHAOS	5		/* mit CHAOS protocols */
#define	AF_NS		6		/* XEROX NS protocols */
#define	AF_ISO		7		/* ISO protocols */
#define	AF_OSI		AF_ISO
#define	AF_ECMA		8		/* european computer manufacturers */
#define	AF_DATAKIT	9		/* datakit protocols */
#define	AF_CCITT	10		/* CCITT protocols, X.25 etc */
#define	AF_SNA		11		/* IBM SNA */
#define AF_DECnet	12		/* DECnet */
#define AF_DLI		13		/* DEC Direct data link interface */
#define AF_LAT		14		/* LAT */
#define	AF_HYLINK	15		/* NSC Hyperchannel */
#define	AF_APPLETALK	16		/* Apple Talk */
#define	AF_ROUTE	17		/* Internal Routing Protocol */
#define	AF_LINK		18		/* Link layer interface */
#define	pseudo_AF_XTP	19		/* eXpress Transfer Protocol (no AF) */
#define	AF_COIP		20		/* connection-oriented IP, aka ST II */
#define	AF_CNT		21		/* Computer Network Technology */
#define pseudo_AF_RTIP	22		/* Help Identify RTIP packets */
#define	AF_IPX		23		/* Novell Internet Protocol */
#define	AF_SIP		24		/* Simple Internet Protocol */
#define pseudo_AF_PIP	25		/* Help Identify PIP packets */

/*
 * We need to know the IPv6 address family number even on IPv4-only systems.
 * Note that this is NOT a protocol constant, and that if the system has its
 * own AF_INET6, different from ours below, all of BIND's libraries and
 * executables will need to be recompiled after the system <sys/socket.h>
 * has had this type added.  The type number below is correct on most BSD-
 * derived systems for which AF_INET6 is defined.
 */
#ifndef AF_INET6
/* SNB: NOTE: This clashes with AF_SIP above.  I've never heard of SIP, and
 * I trust Paul Vixie, so I'm going to leave it in here as 24
 */
#define AF_INET6	24
#endif

#define	AF_MAX		26

/*
 * Protocol families
 */
#define	PF_UNSPEC	AF_UNSPEC
#define	PF_LOCAL	AF_LOCAL
#define	PF_UNIX		PF_LOCAL	/* backward compatibility */
#define PF_INET	    AF_INET   /* Internet */
#define	PF_IMPLINK	AF_IMPLINK
#define	PF_PUP		AF_PUP
#define	PF_CHAOS	AF_CHAOS
#define	PF_NS		AF_NS
#define	PF_ISO		AF_ISO
#define	PF_OSI		AF_ISO
#define	PF_ECMA		AF_ECMA
#define	PF_DATAKIT	AF_DATAKIT
#define	PF_CCITT	AF_CCITT
#define	PF_SNA		AF_SNA
#define PF_DECnet	AF_DECnet
#define PF_DLI		AF_DLI
#define PF_LAT		AF_LAT
#define	PF_HYLINK	AF_HYLINK
#define	PF_APPLETALK	AF_APPLETALK
#define	PF_ROUTE	AF_ROUTE
#define	PF_LINK		AF_LINK
#define	PF_XTP		pseudo_AF_XTP	/* really just proto family, no AF */
#define	PF_COIP		AF_COIP
#define	PF_CNT		AF_CNT
#define	PF_SIP		AF_SIP
#define	PF_IPX		AF_IPX		/* same format as AF_NS */
#define PF_RTIP		pseudo_AF_FTIP	/* same format as AF_INET */
#define PF_PIP		pseudo_AF_PIP
#define PF_INET6	AF_INET6	/* IPv6 */
#define	PF_MAX		AF_MAX

/*
 * 4.3 BSD compatibility structure representing socket addresses
 */
struct sockaddr {
  __u_short sa_family;	      /* Address family */
  char	  sa_data[14];	      /* Address data (maximum 14 bytes) */
};

/*
 * Maximum queue length for listen() calls
 */
#define SOMAXCONN   5

/*
 * Message header for sendmsg() and recvmsg() calls - this is a
 * struct definition kept from 4.3 BSD to preserve compatibility
 * with the Acorn TCP/IP suite.
 */
struct msghdr {
  __caddr_t    msg_name;	 /* Address (optional) */
  int	       msg_namelen;	 /* Address size */
  struct iovec *msg_iov;	 /* Scatter/gather array */
  int	       msg_iovlen;	 /* Number of elements in msg_iov */
  __caddr_t    msg_accrights;	 /* Access rights */
  int	       msg_accrightslen; /* Size of msg_accrights */
};

/*
 * Flags for data sending/receiving calls
 */
#define MSG_OOB		0x1	 /* Process out-of-band data */
#define MSG_PEEK	0x2	 /* Peek at incoming message */
#define MSG_DONTROUTE	0x4	 /* Send without routing */
#define	MSG_EOR		0x8	 /* data completes record */
#define	MSG_TRUNC	0x10	 /* data discarded before delivery */
#define	MSG_CTRUNC	0x20	 /* control data lost before delivery */
#define	MSG_WAITALL	0x40	 /* wait for full request or error */
#define	MSG_DONTWAIT	0x80	 /* this message should be nonblocking */
/*
 * Maximum size of a scatter/gather array
 */
#define MSG_MAXIOVLEN 16

/*
 * Option level for socket level options
 */
#define SOL_SOCKET    0xffff

/*
 * Available socket level options
 */
#define SO_DEBUG      0x0001	 /* Turn on debugging */
#define SO_ACCEPTCONN 0x0002	 /* Socket is listening for connections */
#define SO_REUSEADDR  0x0004	 /* Allow local reuse of this address */
#define SO_KEEPALIVE  0x0008	 /* Keep connections alive */
#define SO_DONTROUTE  0x0010	 /* Bypass routing for this socket */
#define SO_BROADCAST  0x0020	 /* Allow sending of broadcast messages */
#define	SO_USELOOPBACK	0x0040	 /* bypass hardware when possible */
#define SO_LINGER     0x0080	 /* Linger on close if data present */
#define SO_OOBINLINE  0x0100	 /* Receive OOB data inline */
#define	SO_REUSEPORT  0x0200	 /* allow local address & port reuse */

#define SO_SNDBUF     0x1001	 /* Output buffer size */
#define SO_RCVBUF     0x1002	 /* Input buffer size */
#define SO_SNDLOWAT   0x1003	 /* send low-water mark */
#define SO_RCVLOWAT   0x1004	 /* receive low-water mark */
#define SO_SNDTIMEO   0x1005	 /* send timeout */
#define SO_RCVTIMEO   0x1006	 /* receive timeout */
#define SO_ERROR      0x1007	 /* Get and clear error (read only) */
#define SO_TYPE	      0x1008	 /* Get socket type (read only) */

/*
 * structure used for manipulating SO_LINGER option
 */
struct linger {
  int l_onoff;			 /* Option on/off toggle */
  int l_linger;			 /* Time to linger for (in seconds) */
};

/* Create a socket.  */
extern int socket (int af, int type, int protocol);

/* Bind a name to a socket.  */
extern int bind (int s, const struct sockaddr *name, int namelen);

/* Start listening for connections on a socket.  */
extern int listen (int s, int backlog);

/* Accept a connection on a socket.  */
extern int accept (int s, struct sockaddr *name, int *namelen);

/* Make a connection on a socket.  */
extern int connect (int s, const struct sockaddr *name, int namelen);

/* Routines to receive data on a socket. */
extern int recv (int s, void *msg, int len, int flags);
extern int recvfrom (int s, void *msg, int len, int flags,
		    struct sockaddr *from, int *fromlen);
extern int recvmsg (int s, struct msghdr *msg, int flags);

/* Routines to send data from a socket.  */
extern int send (int s, const void *msg, int len, int flags);
extern int sendto (int s, const void *msg, int len, int flags,
		   const struct sockaddr *to, int tolen);
extern int sendmsg (int s, const struct msghdr *msg, int flags);

/* (Partially) shutdown a socket. Perl needs this. Use Perl. Don't use shutdown. */
extern int shutdown (int s, int how);

/* Manipulate socket options.  */
extern int setsockopt (int s, int level, int optname, const void *optval, int optlen);
extern int getsockopt (int s, int level, int optname, void *optval, int *optlen);

/* Find the names of a socket and its peer.  */
extern int getsockname (int s, struct sockaddr *name, int *namelen);
extern int getpeername (int s, struct sockaddr *name, int *namelen);


/* Direct SWI veneers: */
extern int _socket (int af, int type, int protocol);
extern int _bind (int s, const struct sockaddr * name, int namelen);
extern int _listen (int s, int backlog);
extern int _accept (int s, struct sockaddr *name, int *namelen);
extern int _connect (int s, const struct sockaddr *name, int namelen);
extern int _recv (int s, void *msg, int len, int flags);
extern int _recvfrom (int s, void *msg, int len, int flags,
		     struct sockaddr *from, int *fromlen);
extern int _recvmsg (int s, struct msghdr *msg, int flags);
extern int _send (int s, const void *msg, int len, int flags);
extern int _sendto (int s, const void *msg, int len, int flags,
		   const struct sockaddr *to, int tolen);
extern int _sendmsg (int s, const struct msghdr * msg, int flags);
extern int _shutdown (int s, int how);
extern int _setsockopt (int s, int level, int optname,
       	   	        const void *optval, int optlen);
extern int _getsockopt (int s, int level, int optname,
       	   	        char *optval, int *optlen);
extern int _getsockname (int s, struct sockaddr *name, int *namelen);
extern int _getpeername (int s, struct sockaddr *name, int *namelen);

#ifdef __cplusplus
}
#endif

#endif /* socket.h */
