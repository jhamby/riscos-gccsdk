/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/byteorder.h,v $
 * $Date: 2000/07/15 14:52:15 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __SYS_BYTEORDER_H
#define __SYS_BYTEORDER_H

#ifndef __ENDIAN_H
#include <machine/endian.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Convert a 32 bit word from host to net byte order.  */
extern __u_long htonl (__u_long);

/* Convert a 32 bit word from net to host byte order. */
extern __u_long ntohl (__u_long);

/* Convert a 16 bit word from host to net byte order. */
extern __u_short htons (__u_short);

/* Convert a 16 bit word from net to host byte order. */
extern __u_short ntohs (__u_short);

#ifdef __cplusplus
}
#endif

#endif
