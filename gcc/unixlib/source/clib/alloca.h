/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/h/alloca,v $
 * $Date: 1997/10/09 19:59:45 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
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
