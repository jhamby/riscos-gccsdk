/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/debug.h,v $
 * $Date: 2002/09/24 21:02:37 $
 * $Revision: 1.3 $
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

/* __debug(s) dumps UNIX status with title __s.  __s may be NULL when there
   is no title to be printed.  */
extern void __debug(const char *__s);

__END_DECLS

#endif

#endif
