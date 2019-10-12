/* memory.h
 *
 * Copyright 2019 GCCSDK Developers
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

#define ALLOCATOR_ALLOC_FLAG_ALLOC_ONLY	0
#define ALLOCATOR_ALLOC_FLAG_CLAIM	1
#define ALLOCATOR_ALLOC_FLAG_MAP	2
#define ALLOCATOR_ALLOC_FLAGS_MASK	3
#define ALLOCATOR_ALLOC_FLAGS_SHIFT	16

#define ALLOCATOR_ALLOC_FLAGS_ACCESS_MASK	0xffff

#define ALLOCATOR_NAME_MAX_LEN	32

#define ALLOCATOR_HEAP_INITIAL_SIZE	4096

enum block_type
{
  block_type_allocated,
  block_type_free
};

typedef struct memblock
{
  link_hdr link;

  /* Absolute address within dynamic area of this block.  */
  eabi_PTR start_addr;

  /* Absolute address of the end of this block.  */
  eabi_PTR end_addr;

  /* Number of pages in this block.  */
  int page_count;

  enum block_type type;
} memblock;
LINKLIST_ACCESS_FUNCTIONS (memblock)

typedef struct _armeabisupport_allocator
{
  link_hdr link;
  unsigned validate;
  dynamicarea_block da;
  link_list block_list;
  unsigned flags;
  char name[ALLOCATOR_NAME_MAX_LEN];
} armeabisupport_allocator;
LINKLIST_ACCESS_FUNCTIONS (armeabisupport_allocator)

_kernel_oserror *
memory_op(_kernel_swi_regs * r);

_kernel_oserror *
allocator_new(const char *da_name,
	      int max_size,
	      unsigned flags,
	      armeabisupport_allocator **allocator_ret);

_kernel_oserror *
allocator_new_internal(const char *da_name,
		       int max_size,
		       unsigned flags,
		       armeabisupport_allocator **allocator_ret);

_kernel_oserror *
allocator_destroy(armeabisupport_allocator *handle);

_kernel_oserror *
allocator_destroy_internal(armeabisupport_allocator *allocator);

_kernel_oserror *
allocator_alloc(armeabisupport_allocator *handle,
		const int required_page_count,
		unsigned flags,
		eabi_PTR *block_ret);

_kernel_oserror *
allocator_free_internal (armeabisupport_allocator *allocator,
			 memblock *block);

_kernel_oserror *
allocator_free(armeabisupport_allocator *handle,
	       eabi_PTR block);

_kernel_oserror *
allocator_claim_pages(armeabisupport_allocator *handle,
		       eabi_PTR first_page_address,
		       unsigned num_pages);

_kernel_oserror *
allocator_release_pages(armeabisupport_allocator *handle,
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

armeabisupport_allocator *allocator_from_address(eabi_PTR addr);
armeabisupport_allocator *allocator_from_da(int da);
bool allocator_exists(armeabisupport_allocator *allocator);

static inline unsigned int
page_align_size (unsigned int v)
{
  return (v + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
}

static inline eabi_PTR
page_align_addr (eabi_PTR v)
{
  return (eabi_PTR) (((unsigned int) v + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1));
}

static inline int
bytes_to_pages (int v)
{
  return page_align_size (v) >> PAGE_SHIFT;
}

static inline int
pages_to_bytes (int v)
{
  return v << PAGE_SHIFT;
}

static inline int
get_page_number(armeabisupport_allocator *allocator,
		eabi_PTR address)
{
  return (address - (eabi_PTR)allocator->da.base) >> PAGE_SHIFT;
}

#if DEBUG_ALLOCATOR
extern void dump_block_list(armeabisupport_allocator *allocator);
#endif

#endif
