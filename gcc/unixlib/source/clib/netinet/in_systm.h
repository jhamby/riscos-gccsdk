/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/netinet/h/in_systm,v $
 * $Date: 1997/10/09 19:59:52 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
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
