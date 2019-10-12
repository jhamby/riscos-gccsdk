/* memory.c
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#include "armeabisupport.h"
#include "main.h"
#include "memory.h"
#include "swi.h"

#include <stdio.h>
#include <string.h>

#define DEBUG_PRINT 1
#define DEBUG_REPORT 1
#define DEBUG_REPORT_SWI_ERROR 1

#include "debug.h"

static _kernel_oserror *
heap_alloc (armeabisupport_allocator *allocator, int size, eabi_PTR *block_ret);

/* r0 = reason code:
*	0 - new allocator
*	  Entry:
*	   r1 = ptr to name
*	   r2 = max size
*	   r3 = flags
*		bit 0-3 = Type: 1 - page allocator
*				2 - heap allocator
*				3 - single use
*		bit 4   = Set: allocator is global, clear: allocator is specific to current task
*	  Exit:
*	   r0 = handle of new allocator (V flag clear) or error pointer (V flag set)
*	1 - destroy allocator
*	  Entry:
*	   r1 = handle of allocator
*	  Exit:
*	   r0 = 0 for success, or error pointer
*	2 - allocate memory
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = number of pages required
*	   r3 = flags
*		bits 0-15  = Memory page access flags if bits 16,17 = 2 (claim and map)
*		bits 16,17 = 0 - Leave all pages unclaimed and unmapped
*			     1 - claim all pages
*			     2 - claim and map all pages
*	  Exit:
*	   r0 = address of allocated memory block if successful (V flag clear) or error pointer (V flag set)
*	3 - claim pages
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of first page
*	   r3 = number of pages
*	  Exit:
*	   r0 = 0 for success or error pointer
*	4 - release pages
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of first page
*	   r3 = number of pages
*	  Exit:
*	   r0 = 0 for success or error pointer
*	5 - map pages
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of first page
*	   r3 = number of pages
* 	   r4 = memory access flags
*	  Exit:
*	   r0 = 0 for success or error pointer
*	6 - unmap pages
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of first page
*	   r3 = number of pages
*	  Exit:
*	   r0 = 0 for success or error pointer
*	7 - free memory - unmap and release memory pages, then deallocate memory.
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of block to free
*	  Exit:
*	   r0 = 0 for success or error pointer
*	8 - info
*	  Entry:
*	   r1 = handle of allocator
*	  Exit:
*	   r0 = dynamic area number (V clear) or error pointer (V set)
*	   r1 = base address or 0 if error
*	   r2 = end address or 0 if error
*	9 - return page number for given address
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address
*	  Exit:
*	   r0 = page number (V clear) or error pointer (V set)
*	10 - allocator from address
*	  Entry:
*	    r1 = address
*	  Exit:
*	    r0 = handle of allocator, 0 if not found (no error)
*	10 - allocator from dynamic area
*	  Entry:
*	    r1 = dynamic area number
*	  Exit:
*	    r0 = handle of allocator, 0 if not found (no error)
*/

_kernel_oserror *
memory_op(_kernel_swi_regs *r)
{
  _kernel_oserror *err = NULL;
  int reason = r->r[0];

  switch (reason)
  {
    case MEMORYOP_NEW_ALLOCATOR: {
      armeabisupport_allocator *allocator;
      err = allocator_new((const char *)r->r[1], r->r[2], r->r[3], &allocator);
      if (!err)
	r->r[0] = (int)allocator;
      break;
    }
    case MEMORYOP_DESTROY_ALLOCATOR: {
      err = allocator_destroy((armeabisupport_allocator *)r->r[1]);
      break;
    }
    case MEMORYOP_ALLOC: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      eabi_PTR block;
      if (allocator == NULL || allocator->validate != ALLOCATOR_VALIDATE_VALUE)
      {
 	err = armeabisupport_bad_param;
	break;
      }

      switch (allocator->flags & ALLOCATOR_TYPE_MASK)
      {
      case ALLOCATOR_TYPE_PAGE:
	if ((err = allocator_alloc(allocator, r->r[2], (unsigned)r->r[3], &block)) == NULL)
	  r->r[0] = (int)block;
	break;
      case ALLOCATOR_TYPE_HEAP:
	if ((err = heap_alloc(allocator, r->r[2], &block)) == NULL)
	  r->r[0] = (int)block;
	break;
      case ALLOCATOR_TYPE_SINGLE_USE:
	r->r[0] = (int)allocator->da.base;
	break;
      }
      break;
    }
    case MEMORYOP_CLAIM_PAGES: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      eabi_PTR addr = (eabi_PTR)r->r[2];
      unsigned num_pages = (unsigned)r->r[3];

      err = (allocator == NULL || addr == NULL || num_pages == 0) ?
	    armeabisupport_bad_param :
	    allocator_claim_pages(allocator, addr, num_pages);
      break;
    }
    case MEMORYOP_RELEASE_PAGES: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      eabi_PTR addr = (eabi_PTR)r->r[2];
      unsigned num_pages = (unsigned)r->r[3];

      err = (allocator == NULL || addr == NULL || num_pages == 0) ?
	    armeabisupport_bad_param :
	    allocator_release_pages(allocator, addr, num_pages);
      break;
    }
    case MEMORYOP_MAP_PAGES: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      eabi_PTR addr = (eabi_PTR)r->r[2];
      unsigned num_pages = (unsigned)r->r[3];

      err = (allocator == NULL || addr == NULL || num_pages == 0) ?
	    armeabisupport_bad_param :
	    allocator_map_pages(allocator, addr, num_pages, (unsigned)r->r[4]);
      break;
    }
    case MEMORYOP_UNMAP_PAGES: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      eabi_PTR addr = (eabi_PTR)r->r[2];
      unsigned num_pages = (unsigned)r->r[3];

      err = (allocator == NULL || addr == NULL || num_pages == 0) ?
	    armeabisupport_bad_param :
	    allocator_unmap_pages(allocator, addr, num_pages);
      break;
    }
    case MEMORYOP_FREE: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      eabi_PTR addr = (eabi_PTR)r->r[2];

      if (allocator == NULL || allocator->validate != ALLOCATOR_VALIDATE_VALUE || addr == NULL)
      {
	err = armeabisupport_bad_param;
	break;
      }

      switch (allocator->flags & ALLOCATOR_TYPE_MASK)
      {
      case ALLOCATOR_TYPE_PAGE:
	err = allocator_free(allocator, addr);
	break;
      case ALLOCATOR_TYPE_HEAP:
	err = heap_release(allocator->da.base, addr);
	break;
      case ALLOCATOR_TYPE_SINGLE_USE:
	/* Remove the dynamic area?  */
	break;
      }
      break;
    }
    case MEMORYOP_INFO: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      if (allocator == NULL ||
	  !allocator_exists(allocator) ||
	  allocator->validate != ALLOCATOR_VALIDATE_VALUE)
      {
	r->r[1] = r->r[2] = 0;
	err = armeabisupport_bad_param;
	break;
      }

      r->r[0] = allocator->da.number;
      r->r[1] = (int)allocator->da.base;
      r->r[2] = (int)allocator->da.end;
      break;
    }
    case MEMORYOP_PAGE_NUMBER: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      eabi_PTR addr = (eabi_PTR)r->r[2];

      if (allocator == NULL ||
	  addr == NULL ||
	  addr < allocator->da.base ||
	  addr >= allocator->da.end)
      {
	err = armeabisupport_bad_param;
	break;
      }

      r->r[0] = get_page_number(allocator, addr);
      break;
    }
    case MEMORYOP_ALLOCATOR_FROM_ADDRESS: {
      eabi_PTR addr = (eabi_PTR)r->r[1];

      if (addr == NULL)
      {
	err = armeabisupport_bad_param;
	break;
      }

      armeabisupport_allocator *allocator = allocator_from_address(addr);
      r->r[0] = allocator ? (int)allocator : 0;
      break;
    }
    case MEMORYOP_ALLOCATOR_FROM_DA: {
      int da = r->r[1];

      if (da < 256)
      {
	err = armeabisupport_bad_param;
	break;
      }

      armeabisupport_allocator *allocator = allocator_from_da(da);
      r->r[0] = allocator ? (int)allocator : 0;
      break;
    }
    default:
      err = armeabisupport_bad_reason;
      break;
  }

#if DEBUG_ALLOCATOR
  if (err)
  {
    DEBUG_OUTPUT("%s - MemoryOp reason %d",err->errmess,reason);
  }
#endif

  return err;
}

static _kernel_oserror *
heap_alloc (armeabisupport_allocator *allocator, int size, eabi_PTR *block_ret)
{
  bool heap_extended = false;
  _kernel_oserror *err = NULL;

  while (true)
    {
      /* If this fails, then assume heap is full. The failure could be due
         to a corrupted heap or invalid r1, but we can't distinguish
         here.  */
      err = heap_claim (allocator->da.base, size, (void **)block_ret);

      if (err || *block_ret == NULL)
	{
	  if (heap_extended)
	    {
	      /* If we've already extended the heap and it still fails,
	         then don't try again, return the error.  */
	      if (!err)
		err = armeabisupport_no_memory;

	      return err;
	    }

	  /* Attempt to increase heap size and try again. */
	  int heap_inc = (((size * 2) + 0xfff) & ~0xfff);

	  /* Extend DA by (((allocation request * 2) + 0xfff) & ~0xfff)  */
	  if ((err = dynamic_area_extend (allocator->da.number, heap_inc)) != NULL)
	    {
	      DEBUG_OUTPUT_SWI_ERROR(err);
	      return err;
	    }
	  allocator->da.end += heap_inc;

	  /* Extend heap by same amount.  */
	  if ((err = heap_extend (allocator->da.base, heap_inc)) != NULL)
	    return err;

	  heap_extended = true;
	}
      else
	break;
    }

  return NULL;
}

_kernel_oserror *
allocator_claim_pages (armeabisupport_allocator *allocator,
		       eabi_PTR address,
		       unsigned num_pages)
{
  pmp_phy_page_entry *list;
  _kernel_oserror *err;
  int i, pmp_index;

  
  if ((err = rma_claim(sizeof(*list) * num_pages, (void **)&list)) != NULL)
    return err;

  for (i = 0, pmp_index = get_page_number(allocator, address);
       i < num_pages;
       i++, pmp_index++)
    {
      list[i].pmp_page_index = pmp_index;
      list[i].physical_page_number = PMP_PAGE_KERNEL_CHOICE;
      list[i].flags = 0;
    }

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("%s - address: %p, num_pages: %d, pages %d to %d",
		__func__,address,num_pages,
		get_page_number(allocator, address),
		get_page_number(allocator, address) + (num_pages - 1));
#endif

  err = dynamic_pmp_claim_release (allocator->da.number,
				   list,
				   num_pages);
  rma_free (list);

  if (err)
    {
      DEBUG_OUTPUT_SWI_ERROR(err);
    }

  return err;
}

_kernel_oserror *
allocator_release_pages (armeabisupport_allocator *allocator,
			 eabi_PTR address,
			 unsigned num_pages)
{
  pmp_phy_page_entry *list;
  _kernel_oserror *err;
  int i, pmp_index;

  if ((err = rma_claim(sizeof(*list) * num_pages, (void **)&list)) != NULL)
    return err;

  for (i = 0, pmp_index = get_page_number(allocator, address);
       i < num_pages;
       i++, pmp_index++)
    {
      list[i].pmp_page_index = pmp_index;
      list[i].physical_page_number = PMP_PAGE_RELEASE;
      list[i].flags = 0;
    }

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("%s - address: %p, num_pages: %d, pages %d to %d",
		__func__,address,num_pages,
		get_page_number(allocator, address),
		get_page_number(allocator, address) + (num_pages - 1));
#endif

  err = dynamic_pmp_claim_release (allocator->da.number,
				   list,
				   num_pages);
  rma_free(list);

  if (err)
    {
      DEBUG_OUTPUT_SWI_ERROR(err);
    }

  return err;
}

_kernel_oserror *
allocator_map_pages (armeabisupport_allocator *allocator,
		     eabi_PTR address,
		     unsigned num_pages,
		     unsigned flags)
{
  pmp_log_page_entry *list;
  _kernel_oserror *err;
  int i, pmp_index, da_page_num;

  if ((err = rma_claim(sizeof(*list) * num_pages, (void **)&list)) != NULL)
    return err;

  for (i = 0,
        pmp_index = get_page_number(allocator, address),
        da_page_num = get_page_number(allocator, address);
       i < num_pages;
       i++, pmp_index++, da_page_num++)
    {
      list[i].da_page_number = da_page_num;
      list[i].pmp_page_index = pmp_index;
      list[i].flags = flags;
    }

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("%s - address: %p, num_pages: %d, pages %d to %d",
		__func__, address, num_pages,
		get_page_number(allocator, address),
		get_page_number(allocator, address) + num_pages - 1);
#endif

  err = dynamic_pmp_map_unmap (allocator->da.number,
			       list,
			       num_pages);
  rma_free(list);

  if (err)
    {
      DEBUG_OUTPUT_SWI_ERROR(err);
    }

  return err;
}

_kernel_oserror *
allocator_unmap_pages (armeabisupport_allocator *allocator,
		       eabi_PTR address,
		       unsigned num_pages)
{
  pmp_log_page_entry *list;
  _kernel_oserror *err;
  int i, pmp_index, da_page_num;

  if ((err = rma_claim(sizeof(*list) * num_pages, (void **)&list)) != NULL)
    return err;

  for (i = 0,
        pmp_index = get_page_number(allocator, address),
        da_page_num = get_page_number(allocator, address);
       i < num_pages;
       i++, pmp_index++, da_page_num++)
    {
      list[i].da_page_number = da_page_num;
      list[i].pmp_page_index = PMP_PAGE_UNMAP;
      list[i].flags = 0;
    }

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("%s - address: %p, num_pages: %d, pages %d to %d",
	       __func__,
	       address, num_pages,
	       get_page_number(allocator, address),
	       get_page_number(allocator, address) + num_pages - 1);
#endif

  err = dynamic_pmp_map_unmap (allocator->da.number,
			       list,
			       num_pages);
  rma_free(list);

  if (err)
    {
      DEBUG_OUTPUT_SWI_ERROR(err);
    }

  return err;
}

static _kernel_oserror *
DA_extend (dynamicarea_block *da, int by, void **block)
{
  bool heap_extended = false;
  _kernel_oserror *err = NULL;

  while (true)
    {
      /* If this fails, then assume heap is full and attempt to extend it
         (only for +ve extends though). The failure could be due to a
         corrupted heap or invalid heap pointer, but we can't distinguish
         here.  */
      err = heap_extend_block (da->base, block, by);

      /* If an error occurs, then block is not changed, ie it will not be
         NULL.  */
      if (err)
	{
	  if (heap_extended)
	    {
	      /* If we've already extended the heap and it still fails,
	         then don't try again, return the error.  */
	      if (!err)
		err = armeabisupport_no_memory;

	      return err;
	    }

	  /* Attempt to increase heap size and try again.
	     Extend by the size of the block as well as the extra in case
	     the block needs to be moved.  */
	  size_t heap_inc;

	  if ((err = heap_block_size (da->base, *block,
				      &heap_inc)) != NULL)
	    return err;

	  /* Add on the extra and round to 4KB.  */
	  heap_inc = ((heap_inc + by) + 0xfff) & ~0xfff;

	  if ((err = dynamic_area_extend (da->number, heap_inc)) != NULL)
	    {
	      DEBUG_OUTPUT("%s:%d - RISC OS Error: %s", __func__, __LINE__, err->errmess);
	      return err;
	    }
	  da->end += heap_inc;

	  /* Extend heap by same amount.  */
	  if ((err = heap_extend (da->base, heap_inc)) != NULL)
	    return err;

	  heap_extended = true;
	}
      else
	break;
    }

  return NULL;
}

_kernel_oserror *
allocator_new (const char *name,
	       int max_size,
	       unsigned flags,
	       armeabisupport_allocator **allocator_ret)
{
  _kernel_oserror *err;

  app_object *app;
  if ((err = app_find(&app)) != NULL)
    err = app_new(&app);

  if (err)
    return err;

  if ((err = allocator_new_internal(name, max_size, flags, allocator_ret)) != NULL)
    return err;

  link_list *list = (flags & ALLOCATOR_FLAG_GLOBAL) ?
			&global.allocator_list :
			&app->allocator_list;
  linklist_add_to_end(list, &(*allocator_ret)->link);

  return NULL;
}

_kernel_oserror *
allocator_new_internal (const char *name,
			int max_size,
			unsigned flags,
			armeabisupport_allocator **allocator_ret)
{
  if (!name || max_size == 0)
    return armeabisupport_bad_param;

  armeabisupport_allocator *allocator;
  _kernel_oserror *err;

  err = rma_claim(sizeof(*allocator), (void **)&allocator);
  if (err)
    return err;

  memset(allocator, 0, sizeof(*allocator));

  if ((err = dynamic_area_create (name,
				  max_size,
				  flags & ALLOCATOR_TYPE_MASK,
				  &allocator->da)) != NULL)
    goto error;

  if ((flags & ALLOCATOR_TYPE_MASK) == ALLOCATOR_TYPE_HEAP)
    {
      if ((err = heap_init (allocator->da.base, ALLOCATOR_HEAP_INITIAL_SIZE)) != NULL)
	goto error;
    }

  allocator->validate = ALLOCATOR_VALIDATE_VALUE;
  allocator->flags = flags;
  strncpy(allocator->name, name, ALLOCATOR_NAME_MAX_LEN);
  allocator->name[ALLOCATOR_NAME_MAX_LEN - 1] = '\0';

  if (allocator_ret)
    *allocator_ret = allocator;

  return NULL;
error:
  if (allocator->da.number != 0)
    dynamic_area_remove(allocator->da.number);
  rma_free(allocator);
  if (allocator_ret)
    *allocator_ret = NULL;

  return err;
}

_kernel_oserror *
allocator_destroy_internal(armeabisupport_allocator *allocator)
{
  if (allocator == NULL || allocator->validate != ALLOCATOR_VALIDATE_VALUE)
    return armeabisupport_bad_param;

  memblock *block = linklist_first_memblock(&allocator->block_list);
  while (block)
    {
      memblock *next = linklist_next_memblock(block);
      if (block->type == block_type_allocated)
	{
	  /* allocator_free_internal() will also free the RMA block.  */
	  allocator_free_internal(allocator, block);
	}
	else
	{
	  rma_free(block);
	}

	block = next;
     }

  allocator->validate = 0;
  if (allocator->da.number < 256)
    return armeabisupport_bad_param;

  dynamic_area_remove(allocator->da.number);
  rma_free(allocator);

  return NULL;
}

_kernel_oserror *
allocator_destroy(armeabisupport_allocator *allocator)
{
  if (allocator->flags & ALLOCATOR_FLAG_GLOBAL)
    {
      linklist_remove(&global.allocator_list, &allocator->link);
    }
  else
    {
      app_object *app;
      app_find(&app);

      if (app != NULL)
	linklist_remove(&app->allocator_list, &allocator->link);
    }

  return allocator_destroy_internal(allocator);
}

static _kernel_oserror *
add_allocated_block_before (armeabisupport_allocator *allocator,
			    memblock *before_where,
			    eabi_PTR start_addr,
			    int page_count)
{
  memblock *block;
  _kernel_oserror *err;

  if ((err = rma_claim(sizeof (memblock), (void **)&block)) != NULL)
    return err;
  block->start_addr = start_addr;
  block->end_addr = start_addr + pages_to_bytes(page_count);
  block->page_count = page_count;
  block->type = block_type_allocated;

  linklist_add_before (&allocator->block_list,
		       &before_where->link,
		       &block->link);
  return NULL;
}

static _kernel_oserror *
add_allocated_block_to_end (armeabisupport_allocator *allocator,
			    eabi_PTR start_addr,
			    int page_count)
{
  memblock *block;
  _kernel_oserror *err;

  if ((err = rma_claim(sizeof (memblock), (void **)&block)) != NULL)
    return err;
  block->start_addr = start_addr;
  block->end_addr = start_addr + pages_to_bytes(page_count);
  block->page_count = page_count;
  block->type = block_type_allocated;

  linklist_add_to_end (&allocator->block_list,
		       &block->link);
  return NULL;
}

_kernel_oserror *
allocator_alloc (armeabisupport_allocator *allocator,
		 const int required_page_count,
		 unsigned flags,
		 eabi_PTR *block_ret)
{
  _kernel_oserror *err;
  memblock *free_block, *best_fit = NULL;

  *block_ret = NULL;

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("  %s: number pages required (%d)", __func__, required_page_count);

  DEBUG_OUTPUT("  %s: Before alloc:", __func__);
  dump_block_list(allocator);
#endif

  /* Search the block list looking for an ideal fit.
     If we find an exact fit, then we can exit immediately and use that,
     otherwise, find the smallest block.  */
  for (free_block = linklist_first_memblock (&allocator->block_list);
       free_block;
       free_block = linklist_next_memblock (free_block))
    {
      if (free_block->type == block_type_free)
	{
	  if (free_block->page_count == required_page_count)
	    break;
	  else if (required_page_count < free_block->page_count)
	    {
	      if (!best_fit || free_block->page_count < best_fit->page_count)
		best_fit = free_block;
	    }
	}
    }

  if (free_block)
    {
      /* An exact fit was found.  */
      *block_ret = free_block->start_addr;
      free_block->type = block_type_allocated;

#if DEBUG_ALLOCATOR
      DEBUG_OUTPUT("  %s: Exact fit found: address (%p), num_pages (%d)",
			__func__, free_block->start_addr, free_block->page_count);
#endif
    }
  else if (best_fit)
    {
      /* The smallest free block was found.  */
      if ((err = add_allocated_block_before (allocator,
					     best_fit,
					     best_fit->start_addr,
					     required_page_count)) != NULL)
	return err;
      *block_ret = best_fit->start_addr;
      best_fit->start_addr += pages_to_bytes(required_page_count);
      best_fit->page_count -= required_page_count;
#if DEBUG_ALLOCATOR
      DEBUG_OUTPUT("  %s: Best fit found: address (%p), num_pages (%d)",
		    __func__, linklist_prev_memblock(best_fit)->start_addr,
		    linklist_prev_memblock(best_fit)->page_count);
#endif
    }
  else
    {
     /* No free block big enough - extend dynamic area.  */
      dynamicarea_block *da = &allocator->da;
      int extra_pages_required;
      void *block_addr;

      /* If the last block is a free one, then we can extend it.  */
      memblock *last_block =
		linklist_last_memblock (&allocator->block_list);

      if (last_block && last_block->type == block_type_free)
	{
	  extra_pages_required = required_page_count - last_block->page_count;
	  block_addr = last_block->start_addr;
#if DEBUG_ALLOCATOR
	  DEBUG_OUTPUT("  %s: Last block is free: address (%p), num_pages (%d)",
			__func__, last_block->start_addr, last_block->page_count);
#endif
	}
      else
	{
	  extra_pages_required = required_page_count;
	  block_addr = da->end;
	  if ((err = add_allocated_block_to_end (allocator,
						 da->end,
						 required_page_count)) != NULL)
	    return err;
#if DEBUG_ALLOCATOR
	  DEBUG_OUTPUT("  %s: No last block: address (%p)",
			  __func__, block_addr);
#endif
	}
#if 0
FIXME: We need a resize for a normal DA, but not sure about PMP?
      int page_change_count;
      if ((err = dynamic_pmp_resize (da->number,
				     extra_pages_required,
				     &page_change_count)) != NULL)
	{
	  DEBUG_OUTPUT("%s:%d - RISC OS Error: %s, %d, %d",
			__func__, __LINE__,
			err->errmess,
			extra_pages_required,
			page_change_count);

	  /* Undo what was done above. Note that an extra block was added, so the last
	     block has to be found again.  */
	  if (!last_block || last_block->type != block_type_free)
	    {
	      last_block = linklist_last_memblock (&allocator->block_list);
	      linklist_remove (&allocator->block_list, &last_block->link);
	    }
	  return err;
	}

      DEBUG_OUTPUT("  %s: PMP extended by %d pages",
		      __func__, extra_pages_required);
#endif
      *block_ret = block_addr;

      da->end += pages_to_bytes(extra_pages_required);

      if (last_block && last_block->type == block_type_free)
	{
	  last_block->page_count += extra_pages_required;
	  last_block->end_addr += pages_to_bytes(extra_pages_required);
	  last_block->type = block_type_allocated;
	}
    }

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("%s: 0x%p allocated; pages %d to %d (%d pages)",
		__func__,
		*block_ret,
		get_page_number(allocator, *block_ret),
		get_page_number(allocator, *block_ret) + required_page_count - 1,
		required_page_count);

  DEBUG_OUTPUT("  %s: After alloc:", __func__);
  dump_block_list(allocator);
#endif

  /* If it's not a PMP dynamic area, then nothing else to do.  */
  if ((allocator->flags & ALLOCATOR_TYPE_MASK) != ALLOCATOR_TYPE_PAGE)
    return NULL;

  if (((flags >> ALLOCATOR_ALLOC_FLAGS_SHIFT) & ALLOCATOR_ALLOC_FLAGS_MASK) ==
		ALLOCATOR_ALLOC_FLAG_ALLOC_ONLY)
    return NULL;

  err = allocator_claim_pages(allocator,
			      *block_ret,
			      required_page_count);
  if (err)
    return err;
  
  if ((flags >> ALLOCATOR_ALLOC_FLAGS_SHIFT) != ALLOCATOR_ALLOC_FLAG_MAP)
    return NULL;

  return allocator_map_pages(allocator,
			     *block_ret,
			     required_page_count,
			     flags & ALLOCATOR_ALLOC_FLAGS_ACCESS_MASK);
}

/* Step through the pages one by one checking if they're mapped,
 * and releasing and unmapping them if they are.  */
static _kernel_oserror *
unmap_release_pages(armeabisupport_allocator *allocator,
		    memblock *block)
{
  _kernel_oserror *err = NULL;
  pmp_page_info_entry entry;
  int page_entry[3];
  unsigned page;

  page = get_page_number(allocator, block->start_addr);
  while (page < get_page_number(allocator, block->end_addr))
  {
    entry.pmp_page_index = page;
    entry.physical_page_number = -1;
    entry.da_page_number = -1;
    entry.flags = 0;
    err = dynamic_pmp_page_info(allocator->da.number, &entry, 1);
    if (!err && entry.da_page_number != -1)
    {
      page_entry[0] = page;
      page_entry[1] = PMP_PAGE_UNMAP; /* Happens to be same as PMP_PAGE_RELEASE.  */
      page_entry[2] = 0;
      if ((err = dynamic_pmp_map_unmap(allocator->da.number, (pmp_log_page_entry *)page_entry, 1)) == NULL)
	err = dynamic_pmp_claim_release(allocator->da.number, (pmp_phy_page_entry *)page_entry, 1);
    }
    if (err)
      break;
    ++page;
  }

  return err;
}

_kernel_oserror *
allocator_free (armeabisupport_allocator *allocator,
		eabi_PTR block_addr)
{
  memblock *block;

  /* Find the corresponding block for the given address.  */
  for (block = linklist_first_memblock (&allocator->block_list);
       block && block->start_addr != block_addr;
       block = linklist_next_memblock (block))
    /* */;
  
  if (!block)
    return armeabisupport_bad_param;

  return allocator_free_internal(allocator, block);
}

_kernel_oserror *
allocator_free_internal (armeabisupport_allocator *allocator,
			 memblock *block)
{
#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("%s: [%s] Free block (%p) 0x%p -> 0x%p", __func__, allocator->name, block, block->start_addr, block->end_addr);

  DEBUG_OUTPUT("  %s: Before free:", __func__);
  dump_block_list(allocator);
#endif

  if ((allocator->flags & ALLOCATOR_TYPE_MASK) == ALLOCATOR_TYPE_PAGE)
    {
      _kernel_oserror *err;
      err = unmap_release_pages(allocator, block);
      if (err)
	{
	  DEBUG_OUTPUT_SWI_ERROR(err);
	}
    }

  memblock *next = linklist_next_memblock (block);

  if (next && next->type == block_type_free)
    { /* Merge following free block into this one.  */
      block->page_count += next->page_count;
      block->end_addr = next->end_addr;
      block->type = block_type_free;
      linklist_remove (&allocator->block_list, &next->link);
      rma_free (next);
    }
  else
    block->type = block_type_free;

  memblock *prev = linklist_prev_memblock (block);

  if (prev && prev->type == block_type_free)
    { /* Merge this block into previous free block.  */
      prev->page_count += block->page_count;
      prev->end_addr = block->end_addr;
      linklist_remove (&allocator->block_list, &block->link);
      rma_free (block);
    }
  else
    block->type = block_type_free;

  /* If the last block is a free one, prune it off.  */
  block = linklist_last_memblock(&allocator->block_list);
  if (block->type == block_type_free)
    {
      linklist_remove(&allocator->block_list, &block->link);
      rma_free(block);
      /* Record the new end address of the DA.  */
      block = linklist_last_memblock(&allocator->block_list);
      allocator->da.end = block ? block->end_addr : allocator->da.base;
    }

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("  %s: After free:", __func__);
  dump_block_list(allocator);
#endif

  return NULL;
}

armeabisupport_allocator *
allocator_from_address(eabi_PTR addr)
{
  armeabisupport_allocator *allocator;
  app_object *app;

  app_find(&app);
  if (app != NULL)
  {
    for (allocator = linklist_first_armeabisupport_allocator(&app->allocator_list);
	 allocator && (addr < allocator->da.base || addr >= allocator->da.end);
	 allocator = linklist_next_armeabisupport_allocator(allocator))
       /* Empty loop. */;

    if (allocator)
      return allocator;
  }

  for (allocator = linklist_first_armeabisupport_allocator(&global.allocator_list);
       allocator && (addr < allocator->da.base || addr >= allocator->da.end);
       allocator = linklist_next_armeabisupport_allocator(allocator))
     /* Empty loop. */;

  return allocator;
}

armeabisupport_allocator *
allocator_from_da(int da)
{
  armeabisupport_allocator *allocator;
  app_object *app;

  app_find(&app);
  if (app != NULL)
  {
    for (allocator = linklist_first_armeabisupport_allocator(&app->allocator_list);
	 allocator && da != allocator->da.number;
	 allocator = linklist_next_armeabisupport_allocator(allocator))
       /* Empty loop. */;

    if (allocator)
      return allocator;
  }

  for (allocator = linklist_first_armeabisupport_allocator(&global.allocator_list);
       allocator && da != allocator->da.number;
       allocator = linklist_next_armeabisupport_allocator(allocator))
     /* Empty loop. */;

  return allocator;
}

bool allocator_exists(armeabisupport_allocator *allocator)
{
  armeabisupport_allocator *iterator;
  app_object *app;

  app_find(&app);
  if (app != NULL)
  {
    for (iterator = linklist_first_armeabisupport_allocator(&app->allocator_list);
	 iterator && iterator != allocator;
	 iterator = linklist_next_armeabisupport_allocator(iterator))
       /* Empty loop. */;

    if (iterator)
      return true;
  }

  for (iterator = linklist_first_armeabisupport_allocator(&global.allocator_list);
       iterator && iterator != allocator;
       iterator = linklist_next_armeabisupport_allocator(iterator))
     /* Empty loop. */;

  return iterator != NULL;
}

#if DEBUG_ALLOCATOR

void dump_block_list(armeabisupport_allocator *allocator)
{
  DEBUG_OUTPUT("  Allocator: '%s'", allocator->name);
  memblock *block = linklist_first_memblock(&allocator->block_list);
  if (!block)
  {
    DEBUG_OUTPUT("    Empty list");
    return;
  }

  while (block)
  {
    if (block->type == block_type_allocated)
    DEBUG_OUTPUT("    %X -> %X, %d pages allocated", block->start_addr, block->end_addr, block->page_count);
    else
    DEBUG_OUTPUT("    %X -> %X, %d pages free", block->start_addr, block->end_addr, block->page_count);
    block = linklist_next_memblock(block);
  }
}

#endif
