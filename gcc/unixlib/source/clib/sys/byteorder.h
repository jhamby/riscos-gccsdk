/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/byteorder.h,v $
 * $Date: 2002/09/24 21:02:37 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __SYS_BYTEORDER_H
#define __SYS_BYTEORDER_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#ifndef __ENDIAN_H
#include <machine/endian.h>
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
