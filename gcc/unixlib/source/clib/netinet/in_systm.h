/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/netinet/in_systm.h,v $
 * $Date: 2000/07/15 14:52:14 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __NETINET_IN_SYSTM_H
#define __NETINET_IN_SYSTM_H

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

/*
 * Network data types (these are big endian)
 */
typedef __u_short n_short;  /* Short (16 bit word) in net byte order */
typedef __u_long  n_long;   /* Long (32 bit word) in net byte order */
typedef __u_long  n_time;   /* Time in ms since midnight UTC in net byte order */

#endif
