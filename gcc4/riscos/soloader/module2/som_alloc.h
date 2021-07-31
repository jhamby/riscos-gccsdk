/* som_alloc.h
 *
 * Copyright 2007-2019 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_ALLOC_H
#define SOM_ALLOC_H

#include <kernel.h>
#include "link_list.h"
#include "som_os_swis.h"
#include "som_types.h"

#define PAGE_SIZE 0x1000
#define PAGE_SIZE_SHIFT 12
#define PAGE_SIZE_MASK 0xfff

/* Allocate memory from the data only dynamic area.
   If an error occurs, then NULL is returned for the block.  */
extern _kernel_oserror *som_alloc (int size, void **block_ret);

extern _kernel_oserror * som_alloc_lib (int size, void **block_ret);

extern _kernel_oserror *
som_alloc_client_data (int byte_size,
		       som_PTR *block_ret);

/* Extend a previously allocated block.
   If an error occurs, the returned block is the old block, not NULL.  */
extern _kernel_oserror *som_extend (void **block, int by);

extern _kernel_oserror *
armeabi_allocator_alloc (const int required_page_count,
			 unsigned flags,
			 som_PTR *block_ret);
extern _kernel_oserror *
armeabi_allocator_free (som_PTR block_addr);

/* Free memory allocated by either of above SWIs.  */
extern _kernel_oserror *som_free (void *);

static inline unsigned int
page_align_size (unsigned int v)
{
  return (v + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
}

static inline som_PTR
page_align_addr (som_PTR v)
{
  return (som_PTR) (((unsigned int) v + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1));
}

static inline int
bytes_to_pages (int v)
{
  return page_align_size (v) >> PAGE_SIZE_SHIFT;
}

static inline int
pages_to_bytes (int v)
{
  return v << PAGE_SIZE_SHIFT;
}

#endif
