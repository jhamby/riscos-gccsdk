/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/alloca.h,v $
 * $Date: 2002/09/24 21:02:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __ALLOCA_H
#define __ALLOCA_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#define __need_size_t
#include <stddef.h>

__BEGIN_DECLS

/* Allocate a block that will be freed when the calling function exits.  */
extern void *alloca (size_t __size);

__END_DECLS

#endif
