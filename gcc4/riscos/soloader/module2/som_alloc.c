/* som_alloc.c
 *
 * Copyright 2007-2019 GCCSDK Developers
 * Written by Lee Noar
 */

/* The requirement for the libraries to be page aligned makes it impractical
 * to use an allocator ID as used in V2.00.  */

#include "som_alloc.h"
#include "som.h"

#include <stdio.h>

#define DEBUG_PRINT 0
#define DEBUG_REPORT 0

#define DEBUG_ALLOCATOR (DEBUG_PRINT || DEBUG_REPORT)

#if DEBUG_ALLOCATOR
#if DEBUG_PRINT
#define DEBUG_PRINT_IT puts(buffer)
#else
#define DEBUG_PRINT_IT
#endif
#if DEBUG_REPORT
#define DEBUG_REPORT_IT report_text(buffer)
#else
#define DEBUG_REPORT_IT
#endif
#define DEBUG_OUTPUT(format, args...) \
{\
char buffer[256];\
sprintf(buffer,format,args);\
DEBUG_REPORT_IT;\
DEBUG_PRINT_IT;\
}
#else
#define DEBUG_OUTPUT(format, args...)
#endif

#define USE_BLOCK_MARKER 0

static _kernel_oserror *
DA_alloc (dynamic_area_block *da, int size, som_PTR *block_ret)
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
	    {
	      DEBUG_OUTPUT("%s:%d - RISC OS Error: %s",
			   __func__, __LINE__, err->errmess);
	      return err;
	    }
	  da->end_addr += heap_inc;

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

#if USE_BLOCK_MARKER
  size += 4;
#endif

  if ((err = DA_alloc (&global.data_da, size, (som_PTR *)block_ret)) != NULL)
    return err;

  if (*block_ret == NULL)
    return somerr_no_memory;

#if USE_BLOCK_MARKER
  {
    unsigned *ptr = *block_ret;
    *ptr = 0x12345678;
    *block_ret = ptr + 1;
  }
#endif

  return NULL;
}

/* For 32bit, non memory demand paged libraries.  */
_kernel_oserror *
som_alloc_lib (int byte_size, void **block_ret)
{
  _kernel_oserror *err;

  *block_ret = NULL;

  if (global.flags.host_32bit)
    {
      if (global.memory_page_allocator == 0)
	{
	  /* Create the allocator on first use. Creating it during module initialisation
	   * would lead to a startup dependency that is best avoided.  */
	  err = armeabi_memory_new_allocator("Shared Libraries",
					     SOM_MAX_LIB_DA_SIZE,
					     ARMEABISUPPORT_ALLOCATOR_TYPE_PAGE_MAPPED | ARMEABISUPPORT_ALLOCATOR_FLAG_GLOBAL,
					     &global.memory_page_allocator);
	  if (err)
	    return err;
	}

      int page_count = bytes_to_pages(byte_size);
      err = armeabi_memory_alloc(global.memory_page_allocator,
				 page_count,
				 ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAG_MAP | PMP_MEM_ACCESS_RWE_RWE,
				 (som_PTR *)block_ret);
    }
  else
    err = RMA_claim (byte_size, (som_PTR *)block_ret);

  if (err)
    return err;

  if (*block_ret == NULL)
    return somerr_no_memory;

  return NULL;
}

_kernel_oserror *
som_alloc_client_data (int byte_size,
		       som_PTR *block_ret)
{
  _kernel_oserror *err;

  err = armeabi_allocator_alloc (bytes_to_pages (byte_size),
				 ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAG_MAP | PMP_MEM_ACCESS_RWE_RWE,
				 block_ret);

  return err;
}

_kernel_oserror *
som_free (void *block)
{
  if (block == NULL)
    return NULL;

  _kernel_oserror *err;
  armeabi_allocator_t allocator;
  err = armeabi_memory_allocator_from_address(block, &allocator);
  if (!err && allocator)
    err = armeabi_memory_free(allocator, block);
  else if ((som_PTR)block >= global.data_da.base_addr &&
	   (som_PTR)block <  global.data_da.end_addr)
    {
#if USE_BLOCK_MARKER
      unsigned *ptr = (unsigned *)block - 1;
      *ptr = 0;
      block = ptr;
#endif
      err = heap_release (global.data_da.base_addr,
			  block);
    }
    else
      err = RMA_free (block);

  if (err)
    {
      if (allocator)
      {
	int da;
	som_PTR base,end;
	armeabi_memory_info(allocator, &da, &base, &end);
	DEBUG_OUTPUT("  demand paged memory: da = %d base = %p end = %p", da, base, end);
      }
      DEBUG_OUTPUT("som_free(block:%p) (lr:%p)", block,__builtin_return_address(0));
      DEBUG_OUTPUT("  data_da: %p -> %p", global.data_da.base_addr, global.data_da.end_addr);
      DEBUG_OUTPUT("%s:%d - RISC OS Error: %s", __func__, __LINE__, err->errmess);
    }

  return err;
}

static _kernel_oserror *
DA_extend (dynamic_area_block *da, int by, som_PTR *block)
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
	    {
	      DEBUG_OUTPUT("%s:%d - RISC OS Error: %s", __func__, __LINE__, err->errmess);
	      return err;
	    }
	  da->end_addr += heap_inc;

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
  void *b = *block;
  _kernel_oserror *err;

  if (b == NULL)
    return som_alloc (by, block);

  /* Only do extensions for data allocations  */
  if ((som_PTR)b <  global.data_da.base_addr
   || (som_PTR)b >= global.data_da.end_addr)
    return NULL;

#if USE_BLOCK_MARKER
  unsigned *ptr = (unsigned *)b - 1;
  *ptr = 0;
  b = ptr;
#endif

  /* Attempt to extend the heap block.  */
  err = DA_extend (&global.data_da,
		   by,
		   (som_PTR *)&b);

#if USE_BLOCK_MARKER
  ptr = b;
  *ptr = 0x12345678;
  b = ptr + 1;
#endif

  *block = b;

  return err;
}

_kernel_oserror *
armeabi_allocator_alloc (const int required_page_count,
			 unsigned flags,
			 som_PTR *block_ret)
{
  _kernel_oserror *err;

  DEBUG_OUTPUT("  %s: number pages required (%d)", __func__, required_page_count);

  if (global.memory_page_allocator == 0)
    {
      /* Create the allocator on first use. Creating it during module initialisation
       * would lead to a startup dependency that is best avoided.  */
      err = armeabi_memory_new_allocator("Shared Libraries",
					 SOM_MAX_LIB_DA_SIZE,
					 ARMEABISUPPORT_ALLOCATOR_TYPE_PAGE_MAPPED | ARMEABISUPPORT_ALLOCATOR_FLAG_GLOBAL,
					 &global.memory_page_allocator);
      if (err)
	return err;
    }

  err = armeabi_memory_alloc(global.memory_page_allocator,
			     required_page_count,
			     flags,
			     block_ret);
  DEBUG_OUTPUT("%s: 0x%p allocated (%d pages)",
		__func__,
		*block_ret,
		required_page_count);
  return err;

}

_kernel_oserror *
armeabi_allocator_free (som_PTR block_addr)
{
  DEBUG_OUTPUT("%s:%d: Free block 0x%p", __func__, __LINE__, block_addr);

  return armeabi_memory_free(global.memory_page_allocator, block_addr);

}
