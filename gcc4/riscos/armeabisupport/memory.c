/* memory.c
 *
 * Copyright 2019-2021 GCCSDK Developers
 * Written by Lee Noar
 */

#include "armeabisupport.h"
#include "main.h"
#include "memory.h"
#include "mmap.h"
#include "swi.h"
#include "types.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

#define DEBUG_PRINT 0
#define DEBUG_REPORT 1
#define DEBUG_REPORT_SWI_ERROR 1

#include "debug.h"

#define LOGGING 0
#define TRACING 0

#if LOGGING
#define LOG(format,...) reporter_printf(format, ##__VA_ARGS__)
#else
#define LOG(format,...)
#endif

#if TRACING
#define TRACE(format,...) reporter_printf(format, ##__VA_ARGS__)
#else
#define TRACE(format,...)
#endif

#define DECLARE_VT(name) \
static size_t _vt_##name##_size(void); \
static _kernel_oserror * _vt_##name##_new(armeabisupport_allocator *); \
static void _vt_##name##_destroy(armeabisupport_allocator *); \
static _kernel_oserror * _vt_##name##_alloc(armeabi_allocator_t, size_t size, unsigned flags, eabi_PTR *ret); \
static _kernel_oserror * _vt_##name##_free(armeabi_allocator_t, eabi_PTR block, size_t size); \
static void _vt_##name##_dump(armeabisupport_allocator *); \
  \
static const virtual_function_table name##_virtual_funcs = \
{ \
  _vt_##name##_size, \
  _vt_##name##_new, \
  _vt_##name##_destroy, \
  _vt_##name##_alloc, \
  _vt_##name##_free, \
  _vt_##name##_dump, \
  VT_MAGIC \
};

static _kernel_oserror *
allocator_free_internal (armeabisupport_allocator_paged *allocator,
			 memblock *block);

static _kernel_oserror *
heap_alloc (armeabisupport_allocator *allocator, int size, eabi_PTR *block_ret);

static bool allocator_exists(armeabisupport_allocator *);
static armeabisupport_allocator *allocator_from_address(eabi_PTR addr);
static armeabisupport_allocator *allocator_from_da(int da);
static void dump_memblock_list(armeabisupport_allocator *allocator);
static void allocator_dump_page_mapping(armeabisupport_allocator *allocator, eabi_PTR addr, int num_pages);

DECLARE_VT(heap)
DECLARE_VT(paged)
DECLARE_VT(single_use)
DECLARE_VT(mmap)

static inline bool page_allocator_valid(armeabisupport_allocator *allocator)
{
#if VALIDATE_VT
  if (allocator == NULL)
    {
      LOG("page_allocator_valid: NULL allocator");
      return false;
    }
  if (allocator->vt == NULL)
    {
      LOG("page_allocator_valid: NULL allocator virtual table");
      return false;
    }
  if (allocator->vt->magic != VT_MAGIC)
    {
      LOG("page_allocator_valid: Bad allocator virtual table as %p", allocator->vt);
      return false;
    }
  return ((allocator->flags & ALLOCATOR_TYPE_MASK) != ALLOCATOR_TYPE_HEAP);
#else
  return allocator != NULL && allocator->vt &&
	 ((allocator->flags & ALLOCATOR_TYPE_MASK) != ALLOCATOR_TYPE_HEAP);
#endif
}

/* r0 = reason code:
*	0 - new allocator
*	  Entry:
*	   r1 = ptr to name
*	   r2 = max size
*	   r3 = flags
*		bit 0-3 = Type: 1 - page allocator
*				2 - heap allocator
*				3 - single use
*				4 - mmap allocator
*		bit 5   = Set: allocator is global, clear: allocator is specific to current task
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
*	   r2 = number of pages required for the paged allocators or number of bytes for the heap allocator
*	   r3 = flags
*		bits 0-15  = Memory page access flags if bits 16,17 = 2 (claim and map)
*		bits 16,17 = 0 - Leave all pages unclaimed and unmapped
*			     1 - claim all pages
*			     2 - claim and map all pages
*	  Exit:
*	   r0 = address of allocated memory block if successful (V flag clear) or error pointer (V flag set)
*	3 - claim pages (paged allocator only)
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of first page
*	   r3 = number of pages
*	  Exit:
*	   r0 = 0 for success or error pointer
*	4 - release pages (paged allocator only)
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of first page
*	   r3 = number of pages
*	  Exit:
*	   r0 = 0 for success or error pointer
*	5 - map pages (paged allocator only)
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of first page
*	   r3 = number of pages
* 	   r4 = memory access flags
*	  Exit:
*	   r0 = 0 for success or error pointer
*	6 - unmap pages (paged allocator only)
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of first page
*	   r3 = number of pages
*	  Exit:
*	   r0 = 0 for success or error pointer
*	7 - free memory - unmap and release memory pages (if page allocator), then deallocate memory.
*	  Entry:
*	   r1 = handle of allocator
*	   r2 = address of block to free (must be page aligned for the page allocator)
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
*	11 - allocator from dynamic area
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
      armeabi_allocator_t allocator;
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
      size_t size = (size_t)r->r[2];
      if (!page_allocator_valid(allocator) || size == 0)
        {
	  err = armeabisupport_bad_param;
	  break;
        }

      err = allocator->vt->alloc((armeabi_allocator_t)allocator, size, (unsigned)r->r[3], &block);
      if (!err)
	r->r[0] = (int)block;

      break;
    }
    case MEMORYOP_CLAIM_PAGES: {
      eabi_PTR addr = (eabi_PTR)r->r[2];
      unsigned num_pages = (unsigned)r->r[3];

      err = (page_allocator_valid((armeabisupport_allocator *)r->r[1]) && addr != NULL && num_pages != 0) ?
		allocator_claim_pages((armeabisupport_allocator *)r->r[1], addr, num_pages) :
		armeabisupport_bad_param;
      break;
    }
    case MEMORYOP_RELEASE_PAGES: {
      eabi_PTR addr = (eabi_PTR)r->r[2];
      unsigned num_pages = (unsigned)r->r[3];

      err = (page_allocator_valid((armeabisupport_allocator *)r->r[1]) && addr != NULL && num_pages != 0) ?
		allocator_release_pages((armeabisupport_allocator *)r->r[1], addr, num_pages) :
		armeabisupport_bad_param;
      break;
    }
    case MEMORYOP_MAP_PAGES: {
      eabi_PTR addr = (eabi_PTR)r->r[2];
      unsigned num_pages = (unsigned)r->r[3];

      err = (page_allocator_valid((armeabisupport_allocator *)r->r[1]) && addr != NULL && num_pages != 0) ?
		allocator_map_pages((armeabisupport_allocator *)r->r[1], addr, num_pages, (unsigned)r->r[4]) :
		armeabisupport_bad_param;
      break;
    }
    case MEMORYOP_UNMAP_PAGES: {
      eabi_PTR addr = (eabi_PTR)r->r[2];
      unsigned num_pages = (unsigned)r->r[3];

      err = (page_allocator_valid((armeabisupport_allocator *)r->r[1]) && addr != NULL && num_pages != 0) ?
		allocator_unmap_pages((armeabisupport_allocator *)r->r[1], addr, num_pages) :
		armeabisupport_bad_param;
      break;
    }
    case MEMORYOP_FREE: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      eabi_PTR addr = (eabi_PTR)r->r[2];

      if (!page_allocator_valid(allocator) || addr == NULL)
        {
	  err = armeabisupport_bad_param;
	  break;
        }

      err = allocator->vt->free((armeabi_allocator_t)allocator, addr, (size_t)r->r[3]);

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
      r->r[2] = (int)allocator->da.base + pages_to_bytes (allocator->da.size);
      break;
    }
    case MEMORYOP_PAGE_NUMBER: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      eabi_PTR addr = (eabi_PTR)r->r[2];

      if (allocator == NULL ||
	  addr == NULL ||
	  addr < allocator->da.base ||
	  addr >= allocator->da.base + pages_to_bytes (allocator->da.size))
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
      r->r[0] = (int)allocator;
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
    case MEMORYOP_DUMP_PAGE_MAPPINGS: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      if (allocator && allocator_exists(allocator) && allocator->validate == ALLOCATOR_VALIDATE_VALUE)
	allocator_dump_page_mapping(allocator, (eabi_PTR)r->r[2], (size_t)r->r[3]);
      break;
    }
    case MEMORYOP_DUMP_ALLOCATOR: {
      armeabisupport_allocator *allocator = (armeabisupport_allocator *)r->r[1];
      if (allocator && allocator_exists(allocator) && allocator->validate == ALLOCATOR_VALIDATE_VALUE)
	allocator->vt->dump(allocator);
      else
        err = armeabisupport_bad_param;
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
	  int heap_inc = page_align_size(size * 2);

	  /* Extend DA by (((allocation request * 2) + 0xfff) & ~0xfff)  */
	  if ((err = dynamic_area_extend (allocator->da.number, heap_inc)) != NULL)
	    {
	      DEBUG_OUTPUT_SWI_ERROR(err);
	      return err;
	    }
	  allocator->da.size += bytes_to_pages (heap_inc);

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
/*      log_printf(LOG_REPORT,"name: %s, base: 0x%X, max_pages: %d",allocator.base->name,allocator.base->da.base,allocator.base->da.max_size);
      log_printf(LOG_REPORT,"address: 0x%X",address);
      log_printf(LOG_REPORT,"page_number: %d",get_page_number(&allocator.base, address));
      log_printf(LOG_REPORT,"num_pages: 0x%X",num_pages);*/
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

	  /* Add on the extra and round to page size.  */
	  heap_inc = page_align_size(heap_inc + by);

	  if ((err = dynamic_area_extend (da->number, heap_inc)) != NULL)
	    {
	      DEBUG_OUTPUT("%s:%d - RISC OS Error: %s", __func__, __LINE__, err->errmess);
	      return err;
	    }
	  da->size += bytes_to_pages (heap_inc);

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
	       armeabi_allocator_t *allocator_ret)
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
  armeabisupport_allocator *allocator = (armeabisupport_allocator *)(*allocator_ret);
  linklist_add_to_end(list, &allocator->link);

  return NULL;
}

_kernel_oserror *
allocator_new_internal (const char *name,
			int max_size,
			unsigned flags,
			armeabi_allocator_t *allocator_ret)
{
  if (!name || max_size == 0)
    return armeabisupport_bad_param;

  armeabisupport_allocator *allocator;
  _kernel_oserror *err;

  const virtual_function_table *vt;
  switch (flags & ALLOCATOR_TYPE_MASK)
    {
    case ALLOCATOR_TYPE_PAGE:
      vt = &paged_virtual_funcs;
      break;
    case ALLOCATOR_TYPE_HEAP:
      vt = &heap_virtual_funcs;
      break;
    case ALLOCATOR_TYPE_SINGLE_USE:
      vt = &single_use_virtual_funcs;
      break;
    case ALLOCATOR_TYPE_MMAP:
      vt = &mmap_virtual_funcs;
      break;
    default:
      return armeabisupport_bad_param;
    }

  size_t object_size = vt->size();

  err = rma_claim(object_size, (void **)&allocator);
  if (err)
    return err;

  memset(allocator, 0, object_size);
  allocator->vt = vt;

  if ((err = dynamic_area_create (name,
				  max_size,
				  flags & ALLOCATOR_TYPE_MASK,
				  &allocator->da)) != NULL)
    goto error;

  allocator->validate = ALLOCATOR_VALIDATE_VALUE;
  allocator->flags = flags;
  strncpy(allocator->name, name, ALLOCATOR_NAME_MAX_LEN);
  allocator->name[ALLOCATOR_NAME_MAX_LEN - 1] = '\0';

  if (vt->new)
    {
      if ((err = vt->new(allocator)) != NULL)
	goto error;
    }

  if (allocator_ret)
    *allocator_ret = (armeabi_allocator_t)allocator;

  return NULL;
error:
  if (allocator->da.number != 0)
    dynamic_area_remove(allocator->da.number);
  rma_free(allocator);
  if (allocator_ret)
    *allocator_ret = NULL;

  return err;
}

/* Step through the pages one by one checking if they're mapped,
 * and releasing and unmapping them if they are.  */
static _kernel_oserror *
unmap_release_pages(armeabisupport_allocator *allocator,
		    int page,
		    size_t num_pages)
{
  _kernel_oserror *err = NULL;
  pmp_page_info_entry entry;
  int page_entry[3];

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("  unmap_release_pages(addr=%p, num_pages=%d)",
	       page_to_addr(allocator, page),
	       num_pages);
#endif

  if (num_pages < 0)
    return armeabisupport_page_map_error;

  while (num_pages--)
  {
#if DEBUG_ALLOCATOR
    DEBUG_OUTPUT("  unmap_release_pages(page=%d)", page);
#endif
    entry.pmp_page_index = page;//-1;
    entry.physical_page_number = -1;
    entry.da_page_number = -1;//page;
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
      {
        DEBUG_OUTPUT_SWI_ERROR(err);
      }

    page++;
  }

  return err;
}

_kernel_oserror *
allocator_destroy_internal(armeabisupport_allocator *allocator)
{
  if (allocator == NULL || allocator->validate != ALLOCATOR_VALIDATE_VALUE)
    return armeabisupport_bad_param;

  allocator->vt->destroy(allocator);

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
add_allocated_block_before (armeabisupport_allocator_paged *allocator,
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
add_allocated_block_to_end (armeabisupport_allocator_paged *allocator,
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
allocator_alloc (armeabi_allocator_t handle,
		 const int required_page_count,
		 unsigned flags,
		 eabi_PTR *block_ret)
{
  armeabisupport_allocator_paged *allocator = (armeabisupport_allocator_paged *)handle;
  _kernel_oserror *err;
  memblock *free_block, *best_fit = NULL;

  *block_ret = NULL;

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("  %s: number pages required (%d)", __func__, required_page_count);

  DEBUG_OUTPUT("  %s: Before alloc:", __func__);
  dump_memblock_list(allocator);
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
      dynamicarea_block *da = &allocator->base.da;
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
	  block_addr = da->base + pages_to_bytes (da->size);//da->end;
	  if ((err = add_allocated_block_to_end (allocator,
						 da->base + pages_to_bytes (da->size),//da->end,
						 required_page_count)) != NULL)
	    return err;
#if DEBUG_ALLOCATOR
	  DEBUG_OUTPUT("  %s: No last block: address (%p)",
			  __func__, block_addr);
#endif
	}

      *block_ret = block_addr;

      da->size += extra_pages_required;

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
  dump_memblock_list(allocator);
#endif

  /* If it's not a PMP dynamic area, then nothing else to do.  */
  if ((allocator->base.flags & ALLOCATOR_TYPE_MASK) != ALLOCATOR_TYPE_PAGE)
    return NULL;

  if ((flags & ALLOCATOR_ALLOC_FLAGS_MASK) == ALLOCATOR_ALLOC_FLAG_ALLOC_ONLY)
    return NULL;

  err = allocator_claim_pages(&allocator->base,
			      *block_ret,
			      required_page_count);
  if (err)
    return err;
  
  if ((flags & ALLOCATOR_ALLOC_FLAGS_MASK) != ALLOCATOR_ALLOC_FLAG_MAP)
    return NULL;

  return allocator_map_pages(&allocator->base,
			     *block_ret,
			     required_page_count,
			     flags & ALLOCATOR_ALLOC_FLAGS_ACCESS_MASK);
}

_kernel_oserror *
allocator_free (armeabi_allocator_t handle,
		eabi_PTR block_addr)
{
  armeabisupport_allocator_paged *allocator = (armeabisupport_allocator_paged *)handle;
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
allocator_free_internal (armeabisupport_allocator_paged *allocator,
			 memblock *block)
{
#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("%s: [%s] Free block (%p) 0x%p -> 0x%p", __func__, allocator->name, block, block->start_addr, block->end_addr);

  DEBUG_OUTPUT("  %s: Before free:", __func__);
  dump_memblock_list(allocator);
#endif

  if ((allocator->base.flags & ALLOCATOR_TYPE_MASK) == ALLOCATOR_TYPE_PAGE)
    {
      _kernel_oserror *err;
      err = unmap_release_pages(&allocator->base,
				get_page_number(&allocator->base, block->start_addr),
				block->page_count);
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
      allocator->base.da.size -= block->page_count;
      rma_free(block);
    }

#if DEBUG_ALLOCATOR
  DEBUG_OUTPUT("  %s: After free:", __func__);
  dump_memblock_list(allocator);
#endif

  return NULL;
}

_kernel_oserror *
allocator_mmap (armeabisupport_allocator_mmap *allocator,
		const int required_page_count,
		unsigned flags,
		mmap_block **block_ret)
{
  mmap_block *current, *next, *new_block;

  _kernel_oserror *err;
  err = rma_claim(sizeof(mmap_block), (void **)&new_block);
  if (err)
    {
      *block_ret = NULL;
      return err;
    }

  current = linklist_first_mmap_block (&allocator->block_list);
  if (!current || required_page_count <= current->start_page)
    {
      /* If there are no allocations yet, then this is the first block.
       * Otherwise, if there is space before the first block, then we use it by
       * adding the new block before the first one.  */
      new_block->start_page = 0;
      new_block->end_page = required_page_count;
      linklist_add_to_front(&allocator->block_list, &new_block->link);
    }
  else
  {
    /* Go through the list of blocks looking for a gap that we can use
     * between the current block and the next one.  */
    while (current)
    {
      next = linklist_next_mmap_block (current);
      if (!next)
        {
	  /* If we get to the end of the list, then just add the new allocation
	   * to the end of the list.  */
	  if (current->end_page + required_page_count > allocator->base.da.max_size)
	    {
	      /* This allocation would take us beyond the end of the DA, so
	       * clean up and return an error.  */
	      free(new_block);
	      *block_ret = NULL;
	      return armeabisupport_no_memory;
	    }

	  new_block->start_page = current->end_page;
	  new_block->end_page = new_block->start_page + required_page_count;
	  linklist_add_to_end(&allocator->block_list, &new_block->link);
	  break;
        }
      else
        {
	  size_t gap = next->start_page - current->end_page;
	  if (required_page_count <= gap)
	    {
	      /* If we find a gap we can use, then insert the new allocation into the
	       * list.  */
	      new_block->start_page = current->end_page;
	      new_block->end_page = new_block->start_page + required_page_count;
	      linklist_add_before(&allocator->block_list, &next->link, &new_block->link);
	      break;
	    }
        }
      current = next;
    }
  }

  *block_ret = new_block;

  /* Keep the allocator overall size up to date.  */
  if (new_block->end_page > allocator->base.da.size)
    allocator->base.da.size = new_block->end_page;

  if ((flags & ALLOCATOR_ALLOC_FLAGS_MASK) == ALLOCATOR_ALLOC_FLAG_ALLOC_ONLY)
    return NULL;

  eabi_PTR block_address = page_to_addr(&allocator->base, new_block->start_page);
  err = allocator_claim_pages(&allocator->base,
			      block_address,
			      required_page_count);
  if (err)
    return err;

  if ((flags & ALLOCATOR_ALLOC_FLAGS_MASK) != ALLOCATOR_ALLOC_FLAG_MAP)
    return NULL;

  return allocator_map_pages(&allocator->base,
			     block_address,
			     required_page_count,
			     flags & ALLOCATOR_ALLOC_FLAGS_ACCESS_MASK);
}

_kernel_oserror *
allocator_munmap (armeabisupport_allocator_mmap *allocator,
		  eabi_PTR addr,
		  size_t page_count)
{
  /* Make sure the page number can never be -ve, we're using an unsigned for
   * the page number, so we can't test that for a -ve value.  */
  if (addr < allocator->base.da.base)
    return armeabisupport_bad_param;

  uint32_t page = get_page_number(&allocator->base, addr);

  mmap_block *current = linklist_first_mmap_block (&allocator->block_list);
  while (current)
    {
      mmap_block *next = linklist_next_mmap_block (current);
      if (page >= current->start_page && page + page_count <= current->end_page)
        {
	  if (page == current->start_page && page_count == current->end_page - current->start_page)
	    {
	     /* Freeing whole block.  */
	      unmap_release_pages(&allocator->base, page, page_count);
	      linklist_remove(&allocator->block_list, &current->link);
	      rma_free(current);
	    }
	  else if (page == current->start_page)
	    {
	      /* Freeing start of a block.  */
	      unmap_release_pages(&allocator->base, page, page_count);
	      current->start_page += page_count;
	    }
	  else if (page + page_count == current->end_page)
	    {
	      /* Freeing the end of a block.  */
	      current->end_page -= page_count;
	      unmap_release_pages(&allocator->base, current->end_page, page_count);
	    }
	  else
	    {
	      /* Freeing the middle of a block. Creates a new block.  */
	      /* This should actually return ENOMEM from munmap.  */
	      _kernel_oserror *err;
	      mmap_block *new_block;
	      err = rma_claim(sizeof(mmap_block), (void **)&new_block);
	      if (err)
		return err;
	      /* current block is before the hole, new_block is after the hole.
	       * The hole starts at 'page' and extends 'page_count' pages.  */
	      *new_block = *current;
	      new_block->start_page = page + page_count;
	      new_block->end_page = current->end_page;
	      new_block->fd = -1;
	      if (next)
		linklist_add_before(&allocator->block_list, &next->link, &new_block->link);
	      else
		linklist_add_to_end(&allocator->block_list, &new_block->link);
	      /* Release the pages from the hole.  */
	      unmap_release_pages(&allocator->base,
				  page,
				  page_count);
	      current->end_page = page;
	    }
	  break;
        }
      current = next;
    }

  /* Keep the allocator overall size up to date.  */
  current = linklist_last_mmap_block (&allocator->block_list);
  if (current->end_page < allocator->base.da.size)
    allocator->base.da.size = current->end_page;

  return NULL;
}

static _kernel_oserror *
allocator_move_mmap_block(armeabisupport_allocator_mmap *allocator,
			  mmap_block *block,
			  int page_diff,
			  armeabisupport_allocator_mmap **allocator_ret,
			  mmap_block **block_ret)
{
  _kernel_oserror *err;
  armeabisupport_allocator_mmap *new_allocator;
  mmap_block *new_block;

  err = armeabi_mmap(NULL,
		     pages_to_bytes((block->end_page - block->start_page) + page_diff),
		     block->prot,
		     block->flags,
		     block->fd,
		     block->offset,
		     &new_allocator,	/* May be same as the old one.  */
		     &new_block);
  if (err)
    {
      *allocator_ret = NULL;
      *block_ret = NULL;
      return err;
    }

  /* Copy the contents of the old block to the new.  */
  memcpy(page_to_addr(&new_allocator->base, new_block->start_page),
	 page_to_addr(&allocator->base, block->start_page),
	 pages_to_bytes(block->end_page - block->start_page));
  /* Zero fill the new pages.  */
  memset(page_to_addr(&new_allocator->base, new_block->end_page - page_diff),
	 0,
	 pages_to_bytes(page_diff));
  /* Delete the old block.  */
  unmap_release_pages(&allocator->base,
		      block->start_page,
		      block->end_page - block->start_page);
  linklist_remove(&allocator->block_list, &block->link);
  rma_free(block);

  *allocator_ret = new_allocator;
  *block_ret = new_block;

  return NULL;
}

_kernel_oserror *
allocator_mremap(armeabisupport_allocator_mmap *allocator,
		 mmap_block *block,
		 int page_diff,
		 uint32_t flags,
		 armeabisupport_allocator_mmap **allocator_ret,
		 mmap_block **block_ret)
{
  if (page_diff < 0)
    {
      TRACE("allocator_mremap: Reduce mapping by 0x%X pages", -page_diff);

      /* Request to shrink the block. Unmap the pages we no longer need.  */
      block->end_page += page_diff;		/* Note -ve page_diff.  */
      *allocator_ret = allocator;
      *block_ret = block;
      return unmap_release_pages(&allocator->base,
				 block->end_page,
				 -page_diff);	/* Note -ve page_diff, this makes it +ve.  */
    }

  *block_ret = NULL;
  _kernel_oserror *err;
  if (linklist_next_mmap_block(block))
    {
      /* Check if there's space between this and the next block to expand into.  */
      mmap_block *next = linklist_next_mmap_block(block);
      if (next->start_page - block->end_page > page_diff)
        {
	  TRACE("allocator_mremap: Extend block into gap before next block");

	  /* Expand into gap. Claim and Map in the new pages.  */
	  if ((err = allocator_claim_pages(&allocator->base,
					   page_to_addr(&allocator->base, block->end_page),
					   page_diff)) == NULL)
	    err = allocator_map_pages(&allocator->base,
				      page_to_addr(&allocator->base, block->end_page),
				      page_diff,
				      0);
	  if (err)
	    return err;

	  /* Zero fill the new pages.  */
	  memset(page_to_addr(&allocator->base, block->end_page),
		 0,
		 pages_to_bytes(page_diff));
	  block->end_page += page_diff;
	  *allocator_ret = allocator;
	  *block_ret = block;
	  /* FIXME: Need to reapply memory protections after zeroing.  */
	}
      else
	{
	  TRACE("allocator_mremap: Gap before next block not big enough, move to new block");

	  if ((flags & MREMAP_MAYMOVE) == 0)
	    {
	      LOG("allocator_mremap: MREMAP_MAYMOVE not set, not allowed to move block");
	      return armeabisupport_no_memory;
	    }

	  /* Can't expand into gap, have to move to a new block instead.  */
	  return allocator_move_mmap_block(allocator,
					   block,
					   page_diff,
					   allocator_ret,
					   block_ret);
	}
    }
  else
    {
      /* This is the last block, check if there's enough space at the end of the DA
       * to expand into.  */
      if (block->end_page + page_diff > allocator->base.da.max_size)
	{
	  TRACE("allocator_mremap: End of DA, but not enough space, move to new block");

	  if ((flags & MREMAP_MAYMOVE) == 0)
	    {
	      LOG("allocator_mremap: MREMAP_MAYMOVE not set, not allowed to move block");
	      return armeabisupport_no_memory;
	    }

	  /* Extending the block would take us past the end of the DA, have
	   * to move to a new block instead.  */
	  return allocator_move_mmap_block(allocator,
					   block,
					   page_diff,
					   allocator_ret,
					   block_ret);
	}
      else
        {
	  TRACE("allocator_mremap: End of DA, extend into unused space");

	  /* We can extend into the end of the DA. Claim and Map in the new pages.  */
	  if ((err = allocator_claim_pages(&allocator->base,
					   page_to_addr(&allocator->base, block->end_page),
					   page_diff)) == NULL)
	    err = allocator_map_pages(&allocator->base,
				      page_to_addr(&allocator->base, block->end_page),
				      page_diff,
				      0);
	  if (err)
	    return err;

	  /* Zero fill the new pages.  */
	  memset(page_to_addr(&allocator->base, block->end_page),
		 0,
		 pages_to_bytes(page_diff));
	  block->end_page += page_diff;
	  allocator->base.da.size += page_diff;
	  *allocator_ret = allocator;
	  *block_ret = block;
	  /* FIXME: Need to reapply memory protections after zeroing.  */
	}
    }

  return NULL;
}

/* Remove any mmap allocations from the given allocator for the given app.  */
void
allocator_mmap_cleanup_app(armeabisupport_allocator_mmap *allocator,
			   app_object *app)
{
  mmap_block *block = linklist_first_mmap_block (&allocator->block_list);
  while (block)
    {
      mmap_block *next = linklist_next_mmap_block (block);
      if (block->owner == app)
	{
	  unmap_release_pages(&allocator->base,
			      block->start_page,
			      block->end_page - block->start_page);
	  linklist_remove(&allocator->block_list, &block->link);
	  rma_free(block);
	}
      block = next;
    }
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
	 allocator && (addr < allocator->da.base || addr >= (allocator->da.base + pages_to_bytes (allocator->da.size)));
	 allocator = linklist_next_armeabisupport_allocator(allocator))
       /* Empty loop. */;

    if (allocator)
      return allocator;
  }

  for (allocator = linklist_first_armeabisupport_allocator(&global.allocator_list);
       allocator && (addr < allocator->da.base || addr >= (allocator->da.base + pages_to_bytes (allocator->da.size)));
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

static void dump_memblock_list(armeabisupport_allocator *handle)
{
  armeabisupport_allocator_paged *allocator = (armeabisupport_allocator_paged *)handle;
  memblock *block = linklist_first_memblock(&allocator->block_list);
  printf("  Paged allocator: %s\n", allocator->base.name);
  if (!block)
  {
    printf("    Empty list\n");
    return;
  }

  while (block)
  {
    if (block->type == block_type_allocated) 
      printf("    %p -> %p, %d pages allocated\n", block->start_addr, block->end_addr, block->page_count);
    else
      printf("    %p -> %p, %d pages free\n", block->start_addr, block->end_addr, block->page_count);
    block = linklist_next_memblock(block);
  }
}

static void dump_mmap_list(armeabisupport_allocator *handle)
{
  armeabisupport_allocator_mmap *allocator = (armeabisupport_allocator_mmap *)handle;
  mmap_block *current = linklist_first_mmap_block (&allocator->block_list);

  printf("  MMap allocator: %s / %p -> %p\n",
	 allocator->base.name,
	 allocator->base.da.base,
	 page_to_addr(handle, allocator->base.da.size));
  if (!current)
  {
    printf("    Empty list\n");
    return;
  }

  while (current)
    {
      printf("    owner=0x%X; %.8X -> %.8X / %p -> %p / %d pages\n",
	     current->owner->ID,
	     current->start_page,
	     current->end_page,
	     page_to_addr(handle, current->start_page),
	     page_to_addr(handle, current->end_page),
	     current->end_page - current->start_page);
      current = linklist_next_mmap_block (current);
    }
}

void allocator_dump_all(link_list *list)
{
  armeabisupport_allocator *allocator;
  for (allocator = linklist_first_armeabisupport_allocator(list);
       allocator;
       allocator = linklist_next_armeabisupport_allocator(allocator))
    allocator->vt->dump(allocator);
}

void allocator_destroy_all(link_list *list)
{
  armeabisupport_allocator *allocator = linklist_first_armeabisupport_allocator(list);
  while (allocator)
    {
      armeabisupport_allocator *next = linklist_next_armeabisupport_allocator(allocator);
      allocator_destroy_internal(allocator);
      allocator = next;
    }
}

void allocator_dump_page_mapping(armeabisupport_allocator *allocator,
				 eabi_PTR addr,
				 int num_pages)
{
  _kernel_oserror *err = NULL;
  pmp_page_info_entry entry;

  if (num_pages == 0)
    {
      addr = allocator->da.base;
      num_pages = allocator->da.size;
    }

  uint32_t page = get_page_number(allocator, addr);

  char buffer[32];

  while (num_pages > 0)
  {
    char *p = buffer;
    p += sprintf(p, "%p : ", addr);
    int l = num_pages;
    for (int i = 0; i < 16; i++)
      {
        entry.pmp_page_index = page;//-1;
        entry.physical_page_number = -1;
        entry.da_page_number = -1;//page;
        entry.flags = 0;
        err = (void*)dynamic_pmp_page_info(allocator->da.number, &entry, 1);
        if (err)
	  *p++ = 'X';
        else if (entry.da_page_number != -1)
	  *p++ = '1';
        else
	  *p++ = '0';
	if (--l == 0)
	  break;
        page++;
      }
    *p = '\0';
    reporter_printf(buffer);
    num_pages -= 16;
    addr += pages_to_bytes(16);
  }
}

_kernel_oserror *allocator_change_protection(armeabisupport_allocator_mmap *allocator,
					     eabi_PTR addr,
					     size_t num_pages,
					     uint32_t flags)
{
  _kernel_oserror *err = NULL;
  pmp_page_info_entry entry;

  if (num_pages < 0)
    return armeabisupport_page_map_error;

  int page = get_page_number(&allocator->base, addr);
  mmap_block *block;
  for (block = linklist_first_mmap_block (&allocator->block_list);
       block && (page < block->start_page || page >= block->end_page);
       block = linklist_next_mmap_block (block))
    /* Empty loop  */;

  if (!block)
    {
      LOG("allocator_change_protection: Failed to find a block for %p", addr);
      return armeabisupport_page_map_error;
    }

  /* First check if all pages are mapped.  */
  int i = num_pages;
  int p = page;
  while (i--)
  {
    entry.pmp_page_index = p;
    entry.physical_page_number = -1;
    entry.da_page_number = -1;
    entry.flags = 0;
    err = dynamic_pmp_page_info(allocator->base.da.number, &entry, 1);
    if (err || entry.da_page_number == -1)
      {
	if (err)
	  {
	    LOG("allocator_change_protection: dynamic_pmp_page_info error: %s", err->errmess);
	  }
	return armeabisupport_page_map_error;
      }
    p++;
  }

  /* Now change the memory protection.  */
  p = page;
  i = num_pages;
  while (i--)
  {
    pmp_log_page_entry logical_op;
    logical_op.da_page_number = p; /* Same page means don't change the mapping.  */
    logical_op.pmp_page_index = p;
    logical_op.flags = flags;
    if ((err = dynamic_pmp_map_unmap(allocator->base.da.number, &logical_op, 1)) != NULL)
      {
	LOG("allocator_change_protection: dynamic_pmp_map_unmap error: %s", err->errmess);
        return armeabisupport_page_map_error;
      }
    p++;
  }

  block->flags = flags;

  return NULL;
}

static _kernel_oserror *_vt_heap_new(armeabisupport_allocator *allocator)
{
  return heap_init (allocator->da.base, ALLOCATOR_HEAP_INITIAL_SIZE);
}

static void _vt_heap_destroy(armeabisupport_allocator *allocator)
{
}

static size_t _vt_heap_size(void)
{
  return sizeof(armeabisupport_allocator_heap);
}

static _kernel_oserror *_vt_heap_alloc(armeabi_allocator_t allocator, size_t size, unsigned flags, eabi_PTR *ret)
{
  return heap_alloc((armeabisupport_allocator *)allocator, size, ret);
}

static _kernel_oserror *_vt_heap_free(armeabi_allocator_t allocator, eabi_PTR block, size_t size)
{
  return heap_release(((armeabisupport_allocator *)allocator)->da.base, block);
}

static void _vt_heap_dump(armeabisupport_allocator *allocator)
{
  printf("Heap allocator: %s\n", allocator->name);
}

static size_t _vt_paged_size(void)
{
  return sizeof(armeabisupport_allocator_paged);
}

static _kernel_oserror *_vt_paged_new(armeabisupport_allocator *allocator)
{
  return NULL;
}

static void _vt_paged_destroy(armeabisupport_allocator *handle)
{
  armeabisupport_allocator_paged *allocator = (armeabisupport_allocator_paged *)handle;
  memblock *node = linklist_first_memblock (&allocator->block_list);
  while (node)
    {
      memblock *next = linklist_next_memblock(node);
      rma_free(node);
      node = next;
    }

  /* Make sure all memory pages are unmapped and released.  */
  unmap_release_pages (&allocator->base,
		       0,
		       allocator->base.da.size);
}

static _kernel_oserror *_vt_paged_alloc(armeabi_allocator_t handle, size_t num_pages, unsigned flags, eabi_PTR *ret)
{
  return allocator_alloc(handle, num_pages, flags, ret);
}

static _kernel_oserror *_vt_paged_free(armeabi_allocator_t handle, eabi_PTR block, size_t size)
{
  return allocator_free(handle, block);
}

static void _vt_paged_dump(armeabisupport_allocator *allocator)
{
  dump_memblock_list(allocator);
}

static size_t _vt_single_use_size(void)
{
  return sizeof(armeabisupport_allocator_single);
}

static _kernel_oserror *_vt_single_use_new(armeabisupport_allocator *allocator)
{
  return NULL;
}

static void _vt_single_use_destroy(armeabisupport_allocator *allocator)
{
}

static _kernel_oserror *_vt_single_use_alloc(armeabi_allocator_t allocator, size_t num_pages, unsigned flags, eabi_PTR *ret)
{
  *ret = ((armeabisupport_allocator *)allocator)->da.base;
  return NULL;
}

static _kernel_oserror *_vt_single_use_free(armeabi_allocator_t allocator, eabi_PTR block, size_t size)
{
  /* Remove the dynamic area?  */
  return NULL;
}

static void _vt_single_use_dump(armeabisupport_allocator *allocator)
{
  printf("Single use allocator: %s (%p -> %p)\n",
	 allocator->name,
	 allocator->da.base,
	 page_to_addr(allocator, allocator->da.size));
}

static size_t _vt_mmap_size(void)
{
  return sizeof(armeabisupport_allocator_mmap);
}

static _kernel_oserror *_vt_mmap_new(armeabisupport_allocator *allocator)
{
  return NULL;
}

static void _vt_mmap_destroy(armeabisupport_allocator *handle)
{
  armeabisupport_allocator_mmap *allocator = (armeabisupport_allocator_mmap *)handle;
  mmap_block *block = linklist_first_mmap_block (&allocator->block_list);
  while (block)
    {
      mmap_block *next = linklist_next_mmap_block(block);
      rma_free(block);
      block = next;
    }
}

static _kernel_oserror *_vt_mmap_alloc(armeabi_allocator_t allocator,
				       size_t num_pages,
				       unsigned flags,
				       eabi_PTR *ret)
{
  /* mmap allocators are only used internally by the mmap pool. The memory allocation
   * SWI should never end up here.  */
  LOG("_vt_mmap_alloc: Attempt to allocate from mmap pool via SWI");
  return armeabisupport_bad_param;
}

static _kernel_oserror *_vt_mmap_free(armeabi_allocator_t allocator, eabi_PTR block, size_t size)
{
  return allocator_munmap((armeabisupport_allocator_mmap *)allocator, block, size);
}

static void _vt_mmap_dump(armeabisupport_allocator *allocator)
{
  dump_mmap_list(allocator);
}
