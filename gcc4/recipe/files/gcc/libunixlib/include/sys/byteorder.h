/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __SYS_BYTEORDER_H
#define __SYS_BYTEORDER_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __ENDIAN_H
#include <endian.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

__BEGIN_DECLS

/* Convert a 32 bit word from host to net byte order.  */
extern __u_long htonl (__u_long) __THROW __attribute__ ((__const__));

/* Convert a 32 bit word from net to host byte order. */
extern __u_long ntohl (__u_long) __THROW __attribute__ ((__const__));

/* Convert a 16 bit word from host to net byte order. */
extern __u_short htons (__u_short) __THROW __attribute__ ((__const__));

/* Convert a 16 bit word from net to host byte order. */
extern __u_short ntohs (__u_short) __THROW __attribute__ ((__const__));

__END_DECLS

#endif
