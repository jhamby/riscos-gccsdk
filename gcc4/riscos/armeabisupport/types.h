/* types.h
 *
 * Copyright 2019-2021 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef TYPES_H
#define TYPES_H

#include "link_list.h"
#include <stdint.h>

typedef unsigned int bool;
#define true 1
#define false 0

#define ALLOCATOR_NAME_MAX_LEN	32

#define VALIDATE_VT 0

#if VALIDATE_VT
#define VT_MAGIC 0x600D600D
#else
#define VT_MAGIC
#endif

typedef unsigned int stack_t;
typedef unsigned char *eabi_PTR;
typedef void *armeabi_allocator_t;

typedef struct pmp_phy_page_entry
{
  int pmp_page_index;
  int physical_page_number;
  unsigned flags;
} pmp_phy_page_entry;

typedef struct pmp_log_page_entry
{
  int da_page_number;
  int pmp_page_index;
  unsigned flags;
} pmp_log_page_entry;

typedef struct pmp_page_info_entry
{
  int pmp_page_index;
  int physical_page_number;
  int da_page_number;
  unsigned flags;
} pmp_page_info_entry;

typedef struct dynamicarea_block
{
  unsigned int number;
  eabi_PTR base;
  /* Size of DA in pages.  */
  size_t size;
  /* The maximum number of pages available for allocation.
   * base + (max_size - 1) gives the last addressable page. */
  size_t max_size;
} dynamicarea_block;

typedef struct _stack_abort_data_object
{
  link_list *stack_list;
  armeabi_allocator_t *stack_allocator;
} stack_abort_data_object;

typedef struct _app_object
{
  link_hdr link;
  unsigned ID;
  link_list allocator_list;
  link_list abort_handler_list;
  link_list stack_list;
  stack_abort_data_object stack_abort_data;
} app_object;
LINKLIST_ACCESS_FUNCTIONS (app_object)

typedef struct armeabisupport_globals
{
  armeabi_allocator_t stack_allocator;
  link_list mmap_allocator_pool;
  link_list app_list;
  link_list allocator_list;
  bool pre_filter_installed;
  bool post_filter_installed;
  unsigned os_memory_prot_none;
  unsigned os_memory_prot_read;
  unsigned os_memory_prot_read_write;
  unsigned os_memory_prot_exec;
} armeabisupport_globals;

typedef struct _armeabisupport_allocator armeabisupport_allocator;

typedef struct _virtual_function_table
{
  size_t (*size)(void);
  _kernel_oserror * (*new)(armeabisupport_allocator *);
  void (*destroy)(armeabisupport_allocator *);
  _kernel_oserror * (*alloc)(armeabi_allocator_t, size_t size, unsigned flags, eabi_PTR *ret);
  _kernel_oserror * (*free)(armeabi_allocator_t, eabi_PTR block, size_t size);
  void (*dump)(armeabisupport_allocator *);
#if VALIDATE_VT
  uint32_t magic;
#endif
} virtual_function_table;

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

typedef struct mmap_block
{
  link_hdr link;

  uint32_t start_page;	/* Inclusive */
  uint32_t end_page;	/* Non inclusive */

  uint32_t access;	/* Memory protection flags as used by the OS  */

  int prot;		/* Memory protection flags as used by mmap  */
  int flags;		/* Flags as used by mmap  */
  int fd;
  uint32_t offset;

  app_object *owner;
} mmap_block;
LINKLIST_ACCESS_FUNCTIONS (mmap_block)

typedef struct _armeabisupport_allocator
{
  link_hdr link;
  unsigned validate;
  dynamicarea_block da;
  unsigned flags;
  const virtual_function_table *vt;
  char name[ALLOCATOR_NAME_MAX_LEN];
} armeabisupport_allocator;
LINKLIST_ACCESS_FUNCTIONS (armeabisupport_allocator)

typedef struct _armeabisupport_allocator_paged
{
  armeabisupport_allocator base;
  link_list block_list;
} armeabisupport_allocator_paged;

typedef struct _armeabisupport_allocator_single
{
  armeabisupport_allocator base;
} armeabisupport_allocator_single;

typedef struct _armeabisupport_allocator_heap
{
  armeabisupport_allocator base;
} armeabisupport_allocator_heap;

typedef struct _armeabisupport_allocator_mmap
{
  armeabisupport_allocator base;
  link_list block_list;
} armeabisupport_allocator_mmap;

extern armeabisupport_globals global;

#endif
