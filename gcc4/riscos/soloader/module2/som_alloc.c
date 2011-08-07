/* som_alloc.c
 *
 * Copyright 2007-2011 GCCSDK Developers
 * Written by Lee Noar
 */

#include "som_alloc.h"
#include "som.h"

static _kernel_oserror *
DA_alloc (dynamic_area_block *da, int size, void **block_ret)
{
  bool heap_extended = false;
  _kernel_oserror *err = NULL;

  while (true)
    {
      /* If this fails, then assume heap is full. The failure could be due
         to a corrupted heap or invalid r1, but we can't distinguish
         here.  */
      err = heap_claim (da->base_addr, size, block_ret);

      if (err || *block_ret == NULL)
	{
	  if (heap_extended)
	    {
	      /* If we've already extended the heap and it still fails,
	         then don't try again, return the error.  */
	      if (!err)
		err = somerr_no_memory;

	      return err;
	    }

	  /* Attempt to increase heap size and try again. */
	  int heap_inc = (((size * 2) + 0xfff) & ~0xfff);

	  /* Extend DA by (((allocation request * 2) + 0xfff) & ~0xfff)  */
	  if ((err = dynamic_area_extend (da->number, heap_inc)) != NULL)
	    return err;

	  /* Extend heap by same amount.  */
	  if ((err = heap_extend (da->base_addr, heap_inc)) != NULL)
	    return err;

	  heap_extended = true;
	}
      else
	break;
    }

  return NULL;
}

_kernel_oserror *
som_alloc (int size, void **block_ret)
{
  _kernel_oserror *err = NULL;

  *block_ret = NULL;

  /* Add 4 bytes to allocation so we can add an identifier.  */
  if ((err = DA_alloc (&global.data_da, size + 4, block_ret)) != NULL)
    return err;

  if (*block_ret == NULL)
    return somerr_no_memory;

  /* Store an ID word at the start of the memory to show that we allocated
     it.  */
  *((unsigned int *) (*block_ret)) = SOM_ALLOCATOR_SOMX;

  /* Return ptr to address after ID word.  */
  *block_ret += 4;

  return NULL;
}

_kernel_oserror *
som_alloc_lib (int size, void **block_ret)
{
  _kernel_oserror *err;

  *block_ret = NULL;

  /* Add 4 bytes to allocation so we can add an identifier.  */
#ifdef LIBRARIES_IN_DA
  if (global.flags.host_32bit)
    err = DA_alloc (&global.library_da, size + 4, block_ret);
  else
#endif
    err = RMA_claim (size + 4, block_ret);

  if (err)
    return err;

  if (*block_ret == NULL)
    return somerr_no_memory;

  /* Store an ID word at the start of the memory to show that we allocated
     it.  */
  *((unsigned int *) (*block_ret)) = SOM_ALLOCATOR_SOML;

  /* Return ptr to address after ID word.  */
  *block_ret += 4;

  return NULL;
}

_kernel_oserror *
som_free (void *block)
{
  if (block == NULL)
    return NULL;

  unsigned int *b = (unsigned int *) block;
  _kernel_oserror *err;

  /* Find the real start of the block and the allocator ID.  */
  unsigned int allocator = *(--b);

  /* Clear the ID word.  */
  *b = 0;

  /* Validate the allocator ID and ignore if we didn't allocate the memory
     in the first case.  */
  if (allocator == SOM_ALLOCATOR_SOMX)
    err = heap_release (global.data_da.base_addr, b);
  else if (allocator == SOM_ALLOCATOR_SOML)
    {
#ifdef LIBRARIES_IN_DA
      if (global.flags.host_32bit)
	err = heap_release (global.library_da.base_addr, b);
      else
#endif
	err = RMA_free (b);
    }
  else
    err = NULL;			/* Don't return an error in this case. */

  return err;
}

static _kernel_oserror *
DA_extend (dynamic_area_block *da, int by, void **block)
{
  bool heap_extended = false;
  _kernel_oserror *err = NULL;

  while (true)
    {
      /* If this fails, then assume heap is full and attempt to extend it
         (only for +ve extends though). The failure could be due to a
         corrupted heap or invalid heap pointer, but we can't distinguish
         here.  */
      err = heap_extend_block (da->base_addr, block, by);

      /* If an error occurs, then block is not changed, ie it will not be
         NULL.  */
      if (err)
	{
	  if (heap_extended)
	    {
	      /* If we've already extended the heap and it still fails,
	         then don't try again, return the error.  */
	      if (!err)
		err = somerr_no_memory;

	      return err;
	    }

	  /* Attempt to increase heap size and try again.
	     Extend by the size of the block as well as the extra in case
	     the block needs to be moved.  */
	  int heap_inc;

	  if ((err = heap_block_size (da->base_addr, *block,
				      &heap_inc)) != NULL)
	    return err;

	  /* Add on the extra and round to 4KB.  */
	  heap_inc = ((heap_inc + by) + 0xfff) & ~0xfff;

	  if ((err = dynamic_area_extend (da->number, heap_inc)) != NULL)
	    return err;

	  /* Extend heap by same amount.  */
	  if ((err = heap_extend (da->base_addr, heap_inc)) != NULL)
	    return err;

	  heap_extended = true;
	}
      else
	break;
    }

  return NULL;
}

_kernel_oserror *
som_extend (void **block, int by)
{
  unsigned int *b = (unsigned int *) (*block);
  _kernel_oserror *err;

  if (b == NULL)
    return som_alloc (by, block);
    
  unsigned int allocator = *(--b);

  /* Only do extensions for SOMX allocations - other types aren't very
     useful, but could be done.  */
  if (allocator != SOM_ALLOCATOR_SOMX)
    return NULL;

  /* Clear the ID word incase the block moves.  */
  *b = 0;

  /* Attempt to extend the heap block.  */
  err = DA_extend (&global.data_da, by, (void **)(void *)&b);

  /* Reinstate allocator ID. If an error occured above, then this puts the
     allocator ID back in the old block.  */
  *b = allocator;

  /* Return ptr to address after ID word.  */
  *block = b + 1;

  return err;
}
