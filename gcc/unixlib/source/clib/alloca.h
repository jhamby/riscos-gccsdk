/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/alloca.h,v $
 * $Date: 2004/04/12 13:03:37 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __ALLOCA_H
#define __ALLOCA_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#define __need_size_t
#include <stddef.h>

__BEGIN_DECLS

/* Allocate a block that will be freed when the calling function exits.  */
extern void *alloca (size_t __size) __THROW;

__END_DECLS

#endif
