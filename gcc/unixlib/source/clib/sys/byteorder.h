/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/byteorder,v $
 * $Date: 1997/10/09 19:59:53 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
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
