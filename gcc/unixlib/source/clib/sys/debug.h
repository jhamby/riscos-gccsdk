/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/debug.h,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef DEBUG

#ifndef __SYS_DEBUG_H
#define __SYS_DEBUG_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

__BEGIN_DECLS

extern void __debug(const char *);

__END_DECLS

#endif

#endif
