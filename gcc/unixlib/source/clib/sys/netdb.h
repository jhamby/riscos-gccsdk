/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/netdb,v $
 * $Date: 1997/10/09 19:59:55 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __SYS_NETDB_H
#define __SYS_NETDB_H

#ifndef __NETDB_H
#include <netdb.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern struct hostent *_gethostbyname (const char *name);
extern struct hostent *_gethostbyaddr (const char *addr, int len, int type);
extern int _sclose (int sd);
extern int _sread (int sd, void *data, int nbyte);
extern int _swrite (int sd, const void *data, int nbyte);
extern int _sioctl (int sd, int request, void *arg);

#ifdef __cplusplus
}
#endif

#endif
