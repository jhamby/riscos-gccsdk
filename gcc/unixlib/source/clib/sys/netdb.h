/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __SYS_NETDB_H
#define __SYS_NETDB_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __NETDB_H
#include <netdb.h>
#endif

__BEGIN_DECLS

extern struct hostent *_gethostbyname (const char *name) __THROW;
extern struct hostent *_gethostbyaddr (const char *addr,
				       int len, int type) __THROW;
extern int _sclose (int sd) __THROW;
extern int _sread (int sd, void *data, int nbyte) __THROW;
extern int _swrite (int sd, const void *data, int nbyte) __THROW;
extern int _sioctl (int sd, unsigned long request, void *arg) __THROW;

__END_DECLS

#endif
