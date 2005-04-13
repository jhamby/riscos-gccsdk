/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/alloca.h,v $
 * $Date: 2004/10/17 16:24:43 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: joty $
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

#undef alloca

/* Allocate a block that will be freed when the calling function exits.  */
extern void *alloca (size_t __size);

#ifdef __GNUC__
#define alloca(size) __builtin_alloca (size)
#endif

__END_DECLS

#endif
