/* som_alloc.h
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_ALLOC_H
#define SOM_ALLOC_H

#include <kernel.h>

/* Allocate memory from the data only dynamic area.
   If an error occurs, then NULL is returned for the block.  */
_kernel_oserror *som_alloc (int size, void **block_ret);

/* Extend a previously allocated block.
   If an error occurs, the returned block is the old block, not NULL.  */
_kernel_oserror *som_extend (void **block, int by);

/* Allocate memory for library code either from the RMA or dynamic area
   depending on host machine.  */
_kernel_oserror *som_alloc_lib (int size, void **block_ret);

/* Free memory allocated by either of above SWIs.  */
_kernel_oserror *som_free (void *);

#endif
