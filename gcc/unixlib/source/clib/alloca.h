/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/alloca.h,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __ALLOCA_H
#define __ALLOCA_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#ifndef __STDDEF_H
#include <stddef.h>
#endif

__BEGIN_DECLS

/* Allocate a block that will be freed when the calling function exits.  */
extern void *alloca (size_t);

__END_DECLS

#endif
