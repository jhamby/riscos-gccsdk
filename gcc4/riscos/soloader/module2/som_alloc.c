/* som_alloc.c
 *
 * Copyright 2007 GCCSDK Developers
 */

#include "som_alloc.h"
#include "som.h"

static _kernel_oserror *DA_alloc(dynamic_area_block *da, int size, void **block_ret)
{
bool heap_extended = false;
_kernel_oserror *err = NULL;

  while (true)
  {
    /* If this fails, then assume heap is full. The failure could be due to a corrupted heap
       or invalid r1, but we can't distinguish here. */
    err = heap_claim(da->base_addr, size, block_ret);

    if (err || *block_ret == NULL)
    {
      if (heap_extended)
      {
	/* If we've already extended the heap and it still fails, then don't try again,
	   return the error. */
	if (!err)
	  err = somerr_no_memory;

	return err;
      }

      /* Attempt to increase heap size and try again */
    int heap_inc = (((size * 2) + 0xfff) & ~0xfff);

      /* Extend DA by (((allocation request * 2) + 0xfff) & ~0xfff) */
      if ((err = dynamic_area_extend(da->number, heap_inc)) != NULL)
        return err;

      /* Extend heap by same amount */
      if ((err = heap_extend(da->base_addr, heap_inc)) != NULL)
        return err;

      heap_extended = true;
    }
    else
      break;
  }

  return NULL;
}

_kernel_oserror *som_alloc(int size, void **block_ret)
{
_kernel_oserror *err = NULL;

  *block_ret = NULL;

  /* Add 4 bytes to allocation so we can add an identifier */
  if ((err = DA_alloc(&global.data_da, size + 4, block_ret)) != NULL)
    return err;

  if (*block_ret == NULL)
    return somerr_no_memory;

  /* Store an ID word at the start of the memory to show that we allocated it. */
  *((unsigned int *)(*block_ret)) = SOM_ALLOCATOR_SOMX;

  /* Return ptr to address after ID word */
  (*block_ret) += 4;

  return NULL;
}

_kernel_oserror *som_alloc_lib(int size, void **block_ret)
{
_kernel_oserror *err = NULL;

  *block_ret = NULL;

  /* Add 4 bytes to allocation so we can add an identifier */
#ifdef LIBRARIES_IN_DA
  if (global.flags.host_32bit)
    err = DA_alloc(&global.library_da, size + 4, block_ret);
  else
#endif
    err = RMA_claim(size + 4, block_ret);

  if (err)
    return err;

  if (*block_ret == NULL)
    return somerr_no_memory;

  /* Store an ID word at the start of the memory to show that we allocated it. */
  *((unsigned int *)(*block_ret)) = SOM_ALLOCATOR_SOML;

  /* Return ptr to address after ID word */
  (*block_ret) += 4;

  return NULL;
}

_kernel_oserror *som_free(void *block)
{
  if (block == NULL)
    return NULL;

unsigned int *b = (unsigned int *)block;
_kernel_oserror *err = NULL;
unsigned int allocator;

  /* Find the real start of the block and the allocator ID */
  allocator = *(--b);

  /* Clear the ID word. */
  *b = 0;

  /* Validate the allocator ID and ignore if we didn't allocate the memory
     in the first case. */
  if (allocator == SOM_ALLOCATOR_SOMX)
    err = heap_release(global.data_da.base_addr, b);
  else if (allocator == SOM_ALLOCATOR_SOML)
  {
#ifdef LIBRARIES_IN_DA
    if (global.flags.host_32bit)
      err = heap_release(global.library_da.base_addr, b);
    else
#endif
      err = RMA_free(b);
  }
  else
    return NULL; /* Don't return an error in this case */

  return heap_release(global.data_da.base_addr, b);
}
