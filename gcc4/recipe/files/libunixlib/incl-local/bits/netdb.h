/* Internal UnixLib bits/netdb.h
 * Copyright (c) 2008 UnixLib Developers
 */

#include_next <bits/netdb.h>

#ifndef __INTERNAL_BITS_NETDB_H
#define __INTERNAL_BITS_NETDB_H

__BEGIN_DECLS

/* Direct SWI veneers: */
extern int _sclose (int sd);
extern int _sread (int sd, void *data, int nbyte);
extern int _swrite (int sd, const void *data, int nbyte);
extern int _sioctl (int sd, unsigned long request, void *arg);

__END_DECLS

#endif
