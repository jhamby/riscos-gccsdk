/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/debug.h,v $
 * $Date: 2004/09/23 22:16:39 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifdef DEBUG

#ifndef __SYS_DEBUG_H
#define __SYS_DEBUG_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

/* __debug(s) dumps UNIX status with title __s.  __s may be NULL when there
   is no title to be printed.  */
extern void __debug(const char *__s);

__END_DECLS

#endif

#endif
