/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/alloca.h,v $
 * $Date: 2000/07/15 14:52:10 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __ALLOCA_H
#define __ALLOCA_H

#ifndef __STDDEF_H
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Allocate a block that will be freed when the calling function exits.  */
extern void *alloca (size_t);

#ifdef __cplusplus
	}
#endif

#endif
