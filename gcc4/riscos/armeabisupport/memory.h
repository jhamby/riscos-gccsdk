/* memory.h
 *
 * Copyright 2019, 2020 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <kernel.h>
#include "link_list.h"
#include "swis.h"
#include "types.h"

#define DEBUG_ALLOCATOR 0

#define PAGE_SIZE 0x1000
#define PAGE_SHIFT 12
#define PAGE_SIZE_MASK 0xfff

#define ALLOCATOR_TYPE_MASK		0xF
#define ALLOCATOR_TYPE_PAGE		1
#define ALLOCATOR_TYPE_HEAP		2
#define ALLOCATOR_TYPE_SINGLE_USE	3
#define ALLOCATOR_TYPE_MMAP		4
#define ALLOCATOR_FLAG_GLOBAL		(1 << 5)

#define ALLOCATOR_VALIDATE_VALUE 0x58494E55	/* "UNIX" in ASCII.  */

#define MEMORYOP_NEW_ALLOCATOR		0
#define MEMORYOP_DESTROY_ALLOCATOR 	1
#define MEMORYOP_ALLOC			2
#define MEMORYOP_CLAIM_PAGES		3
#define MEMORYOP_RELEASE_PAGES		4
#define MEMORYOP_MAP_PAGES		5
#define MEMORYOP_UNMAP_PAGES		6
#define MEMORYOP_FREE			7
#define MEMORYOP_INFO			8
#define MEMORYOP_PAGE_NUMBER		9
#define MEMORYOP_ALLOCATOR_FROM_ADDRESS	10
#define MEMORYOP_ALLOCATOR_FROM_DA	11
#define MEMORYOP_DUMP_PAGE_MAPPINGS	20
#define MEMORYOP_DUMP_ALLOCATOR		21

#define ALLOCATOR_ALLOC_FLAG_ALLOC_ONLY	(0 << ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ALLOCATOR_ALLOC_FLAG_CLAIM	(1 << ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ALLOCATOR_ALLOC_FLAG_MAP	(2 << ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ALLOCATOR_ALLOC_FLAGS_MASK	(3 << ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ALLOCATOR_ALLOC_FLAGS_SHIFT	16

#define ALLOCATOR_ALLOC_FLAGS_ACCESS_MASK	0xffff

#define ALLOCATOR_HEAP_INITIAL_SIZE	4096

typedef struct _armeabisupport_allocator armeabisupport_allocator;

_kernel_oserror *
memory_op(_kernel_swi_regs * r);

_kernel_oserror *
allocator_new(const char *da_name,
	      int max_size,
	      unsigned flags,
	      armeabi_allocator_t *allocator_ret);

_kernel_oserror *
allocator_new_internal(const char *da_name,
		       int max_size,
		       unsigned flags,
		       armeabi_allocator_t *allocator_ret);

_kernel_oserror *
allocator_destroy(armeabisupport_allocator *handle);

_kernel_oserror *
allocator_destroy_internal(armeabisupport_allocator *allocator);

_kernel_oserror *
allocator_alloc(armeabi_allocator_t handle,
		const int required_page_count,
		unsigned flags,
		eabi_PTR *block_ret);

_kernel_oserror *
allocator_free(armeabi_allocator_t handle,
	       eabi_PTR block);

_kernel_oserror *
allocator_claim_pages(armeabisupport_allocator *allocator,
		      eabi_PTR first_page_address,
		      unsigned num_pages);

_kernel_oserror *
allocator_release_pages(armeabisupport_allocator *allocator,
		        eabi_PTR first_page_address,
		        unsigned num_pages);

_kernel_oserror *
allocator_map_pages (armeabisupport_allocator *allocator,
		     eabi_PTR address,
		     unsigned int num_pages,
		     unsigned flags);

_kernel_oserror *
allocator_unmap_pages (armeabisupport_allocator *allocator,
		       eabi_PTR address,
		       unsigned num_pages);

_kernel_oserror *
allocator_mmap (armeabisupport_allocator_mmap *allocator,
		const int required_page_count,
		unsigned flags,
		mmap_block **block_ret);
_kernel_oserror *
allocator_munmap(armeabisupport_allocator_mmap *handle,
		 eabi_PTR block, size_t num_pages);
_kernel_oserror *
allocator_mremap(armeabisupport_allocator_mmap *allocator,
		 mmap_block *block,
		 int page_diff,
		 uint32_t flags,
		 armeabisupport_allocator_mmap **allocator_ret,
		 mmap_block **block_ret);

_kernel_oserror *
allocator_change_protection(armeabisupport_allocator_mmap *allocator,
			    eabi_PTR addr,
			    size_t num_pages,
			    uint32_t flags);

void allocator_destroy_all(link_list *list);

/* Remove any mmap allocations from the given allocator for the given app.  */
void
allocator_mmap_cleanup_app(armeabisupport_allocator_mmap *allocator,
			   app_object *app);

static inline size_t
page_align_size (size_t v)
{
  return (v + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
}

static inline eabi_PTR
page_align_addr (eabi_PTR v)
{
  return (eabi_PTR) (((unsigned int) v + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1));
}

static inline int
bytes_to_pages (size_t v)
{
  return (int)page_align_size (v) >> PAGE_SHIFT;
}

static inline size_t
pages_to_bytes (int v)
{
  return (size_t)(v << PAGE_SHIFT);
}

/* Return TRUE if the number of bytes is a multiple of the page size.  */
static inline int is_whole_pages(size_t bytes)
{
  return (bytes & (PAGE_SIZE - 1)) == 0;
}

/* Return TRUE if the address is page aligned.  */
static inline int is_page_aligned(eabi_PTR addr)
{
  return ((unsigned)addr & (PAGE_SIZE - 1)) == 0;
}

static inline eabi_PTR page_to_addr(armeabisupport_allocator *allocator, uint32_t page)
{
  return allocator->da.base + pages_to_bytes(page);
}

static inline int
get_page_number(armeabisupport_allocator *allocator,
		eabi_PTR address)
{
  return (address - (eabi_PTR)allocator->da.base) >> PAGE_SHIFT;
}

extern void allocator_dump_all(link_list *list);

#endif
