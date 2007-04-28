/* som_alloc.h
 *
 * Copyright 2007 GCCSDK Developers
 */

#ifndef SOM_ALLOC_H
#define SOM_ALLOC_H

#ifndef __KERNEL_H
#include <kernel.h>
#endif

/* Allocate memory from the data only dynamic area. */
_kernel_oserror *som_alloc(int size, void **block_ret);

/* Allocate memory for library code either from the RMA or dynamic area
   depending on host machine. */
_kernel_oserror *som_alloc_lib(int size, void **block_ret);

/* Free memory allocated by either of above SWIs. */
_kernel_oserror *som_free(void *);

#endif
