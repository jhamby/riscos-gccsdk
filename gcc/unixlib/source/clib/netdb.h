/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/netdb.h,v $
 * $Date: 2000/07/15 14:52:11 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __NETDB_H
#define __NETDB_H 1

#ifndef _UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Database entry for a single host. */
struct hostent
{
  /* Official name of host.  */
  char *h_name;
  /* Alias list.  */
  char **h_aliases;
  /* Host address type.  */
  int h_addrtype;
  /* Length of address.  */
  int h_length;
  /* List of addresses from name server.  */
  char **h_addr_list;
  /* Address, for backward compatiblity.  */
#define h_addr h_addr_list[0]
};

extern int sethostent (int __stayopen);
extern struct hostent *gethostent (void);
extern int endhostent (void);
extern struct hostent *gethostbyname (const char *__name);
extern struct hostent *_gethostbyname (const char *__name);
extern struct hostent *gethostbyaddr (const char *__addr, int __len, int __type);
extern struct hostent *_gethostbyaddr (const char *__addr, int __len, int __type);


/* Structure describing a network entry.  */
struct netent
{
  /* Official name of this network.  */
  char *n_name;
  /* Alias list.  */
  char **n_aliases;
  /* Network address type.  */
  int n_addrtype;
  /* Network number.  */
  __u_long n_net;
};

extern int setnetent (int __stayopen);
extern struct netent *getnetent (void);
extern int endnetent (void);
extern struct netent *getnetbyname (const char *__name);
extern struct netent *getnetbyaddr (long __net, int __type);

/* Database entry for a service.  */
struct servent
{
  /* Official service name.  */
  char *s_name;
  /* Alias list.  */
  char **s_aliases;
  /* Port number.  */
  int s_port;
  /* Protocol to use.  */
  char *s_proto;
};

extern int setservent (int __stayopen);
extern struct servent *getservent (void);
extern int endservent (void);
extern struct servent *getservbyname (const char *__name, const char *__proto);
extern struct servent *getservbyport (int __port, const char *__proto);

/* Database structure for a protocol entry.  */
struct protoent
{
  /* Official protocol name.  */
  char *p_name;
  /* Alias list.  */
  char **p_aliases;
  /* Protocol number.  */
  int p_proto;
};

extern int setprotoent (int __stayopen);
extern struct protoent *getprotoent (void);
extern int endprotoent (void);
extern struct protoent *getprotobyname (const char *__name);
extern struct protoent *getprotobynumber (int __proto);

struct rpcent
{
  /* Name of server for this rpc program.  */
  char *r_name;
  /* Alias list.  */
  char **r_aliases;
  /* rpc program number.  */
  int r_number;
};

extern int setrpcent (int __stayopen);
extern struct rpcent *getrpcent (void);
extern int endrpcent (void);
extern struct rpcent *getrpcbyname (const char *__name);
extern struct rpcent *getrpcbynumber (int __proto);

/* InetDB and Stewart Brodie's resolver do not return codes for h_errno.  */

/* Error return codes from gethostbyname() and gethostbyaddr()
   (left in extern int h_errno).  */

/* Authoritative Answer Host not found */
#define HOST_NOT_FOUND	1
/* Non-Authoritive Host not found, or SERVERFAIL */
#define TRY_AGAIN	2
/* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
#define NO_RECOVERY	3
/* Valid name, no data record of requested type */
#define NO_DATA		4
/* no address, look for MX record */
#define NO_ADDRESS	NO_DATA

#ifdef __cplusplus
}
#endif

#endif
