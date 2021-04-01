/* mmap.c
 *
 * Copyright 2021 GCCSDK Developers
 * Written by Lee Noar
 */

#include "armeabisupport.h"
#include "mmap.h"
#include "memory.h"
#include "abort.h"
#include "main.h"
#include "swi.h"
#include "debug.h"
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#define LOGGING 0
#define TRACING 0

#define MMAP_MAX_DA_SIZE (100 * 1024 * 1024)

#define MMAPOP_MAP	0
#define MMAPOP_UNMAP	1
#define MMAPOP_ADVISE	2
#define MMAPOP_PROTECT	3
#define MMAPOP_GET_INFO	4
#define MMAPOP_REMAP	5
#define MMAPOP_GET_PROT	6
#define MMAPOP_DUMP	16

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

static _kernel_oserror *
armeabi_munmap(eabi_PTR addr,
	       size_t size);
static _kernel_oserror *
armeabi_madvise(eabi_PTR addr,
		size_t size,
		int advice);
static _kernel_oserror *
armeabi_mprotect(void *addr,
		 size_t len,
		 int prot);
static void mmap_dump(void);
static _kernel_oserror *
armeabi_get_info(eabi_PTR addr,
		 eabi_PTR *mmap_addr,
		 size_t *size,
		 int *prot,
		 int *flags,
		 int *handle,
		 size_t *offset);
static _kernel_oserror *
armeabi_mremap (eabi_PTR addr,
		size_t old_len,
		size_t new_len,
		int flags,
		armeabisupport_allocator_mmap **allocator_ret,
		mmap_block **block_ret);
static _kernel_oserror *
armeabi_get_page_protection (eabi_PTR addr,
			     int *prot_ret);

/*  r0 = reason code
*	0 - Map
*	  Entry:
*	    r1 = address (usually NULL, otherwise must point to existing block and even then can
*			  only be used to change protection)
*	    r2 = length of mapping in bytes
*	    r3 = protection flags as used by mmap
*	    r4 = mmap flags
*	    r5 = fd
*	    r6 = offset
*	Exit:
*	    r0 = page aligned address of allocated block if successful (V flag clear) or error pointer (V flag set)
*	1 - Unmap
*	  Entry:
*	    r1 = address - must exist in an existing block, but does not have to be its start address
*	    r2 = length of mapping in bytes - address + length must be contained within an existing block
*	  Exit:
*	    r0 = 0 if successful (V flag clear) or error pointer (V flag set)
*	2 - Advise
*	  Entry:
*	    r1 = address
*	    r2 = number of bytes
*	    r3 = advice - as per madvise
*	  Exit:
*	    r0 = 0 if successful (V flag clear) or error pointer (V flag set)
*	3 - Protect
*	  Entry:
*	    r1 = address
*	    r2 = number of bytes
*	    r3 = prot - as per mprotect
*	  Exit:
*	    r0 = 0 if successful (V flag clear) or error pointer (V flag set)
*	4 - GetInfo
*	  Entry:
*	    r1 = address within an existing block
*	  Exit:
*	    r0 = address of start of block if successful (V flag clear) or error pointer (V flag set)
*	    r1 = length of block in bytes
*	    r2 = mmap protection flags when block was allocated
*	    r3 = mmap flags used when block was allocated
*	    r4 = mmap fd given when block was allocated
*	    r5 = mmap offset given when block was allocated
*	5 - Remap
*	  Entry:
*	    r1 = address of block to remap
*	    r2 = old length of block in bytes
*	    r3 = new length of block in bytes
*	    r4 = flags as mremap
*	  Exit:
*	    r0 = address of block's new address if successful (V flag clear) or error pointer (V flag set)
*	6 - Get Protection
*	  Entry:
*	    r1 = address within page to get protection flags of
*	  Exit:
*	    r0 = mmap protection flags for page containing address given
*/

_kernel_oserror *
mmap_op(_kernel_swi_regs *r)
{
  _kernel_oserror *err = NULL;
  switch (r->r[0])
  {
    case MMAPOP_MAP: {
      armeabisupport_allocator_mmap *allocator;
      mmap_block *block;
      err = armeabi_mmap((eabi_PTR)r->r[1],
			 (size_t)r->r[2],
			 r->r[3],
			 r->r[4],
			 r->r[5],
			 (size_t)r->r[6],
			 &allocator,
			 &block);
      if (!err)
	r->r[0] = (int)page_to_addr(&allocator->base, block->start_page);
      break;
    }
    case MMAPOP_UNMAP:
      err = armeabi_munmap((eabi_PTR)r->r[1],
			   (size_t)r->r[2]);
      break;
    case MMAPOP_ADVISE:
      err = armeabi_madvise((eabi_PTR)r->r[1],
			    (size_t)r->r[2],
			    r->r[3]);
      break;
    case MMAPOP_PROTECT:
      err = armeabi_mprotect((eabi_PTR)r->r[1],
			     (size_t)r->r[2],
			     r->r[3]);
      break;
    case MMAPOP_GET_INFO: {
      eabi_PTR map_addr;
      size_t size, offset;
      int prot, flags, handle;
      err = armeabi_get_info((eabi_PTR)r->r[1],
			     &map_addr,
			     &size,
			     &prot,
			     &flags,
			     &handle,
			     &offset);
      if (!err)
        {
	  r->r[0] = (int)map_addr;
	  r->r[1] = (int)size;
	  r->r[2] = prot;
	  r->r[3] = flags;
	  r->r[4] = handle;
	  r->r[5] = (int)offset;
	}
      break;
    }
    case MMAPOP_REMAP: {
      armeabisupport_allocator_mmap *allocator;
      mmap_block *block;

      err = armeabi_mremap((eabi_PTR)r->r[1],
			   (size_t)r->r[2],
			   (size_t)r->r[3],
			   r->r[4],
			   &allocator,
			   &block);
      if (!err)
	r->r[0] = (int)page_to_addr(&allocator->base, block->start_page);
      break;
    }
    case MMAPOP_GET_PROT: {
      int prot;
      err = armeabi_get_page_protection((eabi_PTR)r->r[1],
					&prot);
      if (!err)
	r->r[0] = prot;
      break;
    }
    case MMAPOP_DUMP:
      mmap_dump();
      break;
  }

  return err;
}

static inline armeabisupport_allocator_mmap *
get_allocator(const eabi_PTR addr)
{
  armeabisupport_allocator_mmap *allocator;
  for (allocator = (armeabisupport_allocator_mmap *)linklist_first_armeabisupport_allocator(&global.mmap_allocator_pool);
       allocator && (addr < allocator->base.da.base || addr >= (allocator->base.da.base + pages_to_bytes (allocator->base.da.size)));
       allocator = (armeabisupport_allocator_mmap *)linklist_next_armeabisupport_allocator(&allocator->base))
       /* Empty loop. */;

  return allocator;
}

static inline mmap_block *
get_block(armeabisupport_allocator_mmap *allocator, eabi_PTR addr)
{
  int page = get_page_number(&allocator->base, addr);
  mmap_block *block;
  for (block = linklist_first_mmap_block (&allocator->block_list);
       block && (page < block->start_page || page >= block->end_page);
       block = linklist_next_mmap_block (block))
    /* Empty loop  */;

  return block;
}

static _kernel_oserror *convert_protection(int prot, uint32_t *access_flags_ret)
{
  uint32_t access_flags = 0;
  if (prot & PROT_READ)
    access_flags |= OS_PERMISSION_FLAG_USER_READ;
  if (prot & PROT_WRITE)
    access_flags |= OS_PERMISSION_FLAG_USER_WRITE;
  if (prot & PROT_EXEC)
    access_flags |= OS_PERMISSION_FLAG_USER_EXEC;

  return get_os_permissions(access_flags,
			    OS_PERMISSION_FLAG_USER_READ |
				OS_PERMISSION_FLAG_USER_WRITE |
				OS_PERMISSION_FLAG_USER_EXEC,
			    access_flags_ret);
}

/* We only support addr being non-null when it refers to an existing block and we are changing
 * the protection.  */
static _kernel_oserror *
armeabi_mmap_non_null_addr(eabi_PTR addr, size_t size, int prot, armeabisupport_allocator_mmap **allocator_ret, mmap_block **block_ret)
{
  TRACE("armeabi_mmap_non_null_addr(%p, %d, %d)", addr, size, prot);

  _kernel_oserror *err;
  armeabisupport_allocator_mmap *allocator = get_allocator(addr);
  if (!allocator)
    {
      err = armeabisupport_bad_param;
      goto error;
    }

  mmap_block *block = get_block(allocator, addr);
  if (!block)
    {
      err = armeabisupport_bad_param;
      goto error;
    }

  int page = get_page_number(&allocator->base, addr);
  uint32_t num_pages = bytes_to_pages(size);

  if (page + num_pages > block->end_page)
    {
      err = armeabisupport_page_map_error;
      goto error;
    }

  uint32_t access_flags;
  if ((err = convert_protection(prot, &access_flags)) != NULL)
    goto error;

  /* Record the new protection only if it's for the whole block.  */
  if (page == block->start_page && num_pages == block->end_page - block->start_page)
    {
      block->prot = prot;
      block->access = access_flags;
    }

  *allocator_ret = allocator;
  *block_ret = block;
  return allocator_change_protection(allocator, addr, num_pages, access_flags);

error:
  *allocator_ret = NULL;
  *block_ret = NULL;
  return err;
}

_kernel_oserror *
armeabi_mmap(eabi_PTR addr, size_t size, int prot, int flags, int fd, size_t offset,
	     armeabisupport_allocator_mmap **allocator_ret, mmap_block **block_ret)
{
  TRACE("armeabi_mmap(size:%d, prot:%d, flags:%d, fd:%d, offset:%d)",
	size, prot, flags, fd, offset);

  if (size == 0)
    return armeabisupport_bad_param;

  if (addr != NULL)
    return armeabi_mmap_non_null_addr(addr, size, prot, allocator_ret, block_ret);

  if (flags & MAP_FIXED)
    return armeabisupport_bad_param;

  armeabisupport_allocator *allocator;
  _kernel_oserror *err;

  *allocator_ret = NULL;
  *block_ret = NULL;

  bool new_allocator = false;
  uint32_t num_pages = bytes_to_pages(size);

  allocator = linklist_first_armeabisupport_allocator(&global.mmap_allocator_pool);
  while (true)
  {
    if (!allocator)
      {
	/* If we've already tried creating a new allocator and that failed, then bail.  */
	if (new_allocator)
	  {
	    LOG("armeabi_mmap: Failed to allocate %d pages after creating new allocator", num_pages);
	    break;
	  }

	static int mmap_id = 0;
	char buffer[16];
	memset(buffer, 0, 16);
	snprintf(buffer, 16, "mmap#%d", mmap_id++);

	TRACE("armeabi_mmap: Creating new allocator %s", buffer);
	/* End of allocator list or list is empty, either way, create a new allocator.  */
	err = allocator_new_internal(buffer,
				     MMAP_MAX_DA_SIZE,
				     ALLOCATOR_TYPE_MMAP | ALLOCATOR_FLAG_GLOBAL,
				     (void **)&allocator);
	if (err)
	  return err;

	new_allocator = true;
	linklist_add_to_end(&global.mmap_allocator_pool, &allocator->link);
      }

    mmap_block *block;
    err = allocator_mmap((armeabisupport_allocator_mmap *)allocator,
			 num_pages,
			 ALLOCATOR_ALLOC_FLAG_MAP,
			 &block);
    if (!err && block != NULL)
      {
	/* Record the owner of this block.  */
	app_find(&block->owner);
	block->fd = fd;
	block->flags = flags;
	block->prot = prot;
	block->offset = offset;

	*allocator_ret = (armeabisupport_allocator_mmap *)allocator;
	*block_ret = block;
        TRACE("armeabi_mmap: Allocated %d pages from allocator %s\n", num_pages, allocator->name);
        return NULL;
      }

    /* This allocator can't allocate the memory, try the next one.  */
    allocator = linklist_next_armeabisupport_allocator(allocator);
  }

  return armeabisupport_no_memory;
}

_kernel_oserror *
armeabi_munmap(eabi_PTR addr, size_t size)
{
  TRACE("armeabi_munmap(%p, %d (0x%X))\n", addr, size, size);

  uint32_t num_pages = bytes_to_pages(size);

  armeabisupport_allocator_mmap *allocator = get_allocator(addr);
  if (!allocator)
    {
      LOG("armeabi_munmap: Failed to find address in allocator pool\n");
      return armeabisupport_bad_param;
    }

  _kernel_oserror *err = allocator_munmap(allocator, addr, num_pages);
  if (err)
    return err;

  if (allocator->block_list.count == 0)
    {
      TRACE("armeabi_munmap: Allocator %s empty, destroying to reclaim dynamic area\n", allocator->base.name);
      /* No more allocations in this allocator, remove from the pool.  */
      linklist_remove(&global.mmap_allocator_pool, &allocator->base.link);
      allocator_destroy_internal(&allocator->base);
    }

  return NULL;
}

_kernel_oserror *
armeabi_madvise(eabi_PTR addr, size_t size, int advice)
{
  armeabisupport_allocator_mmap *allocator;
  for (allocator = (armeabisupport_allocator_mmap *)linklist_first_armeabisupport_allocator(&global.mmap_allocator_pool);
       allocator && (addr < allocator->base.da.base || addr >= (allocator->base.da.base + pages_to_bytes (allocator->base.da.size)));
       allocator = (armeabisupport_allocator_mmap *)linklist_next_armeabisupport_allocator(&allocator->base))
       /* Empty loop. */;

  if (!allocator)
    {
      LOG("armeabi_madvise: Failed to find address in allocator pool\n");
      return armeabisupport_bad_param;
    }

  _kernel_oserror *err;
  uint32_t num_pages = bytes_to_pages(size);
  if (advice == MADV_DONTNEED || advice == MADV_FREE)
    {
      if ((err = allocator_unmap_pages(&allocator->base, addr, num_pages)) == NULL)
        err = allocator_release_pages(&allocator->base, addr, num_pages);
    }
  else if (advice == MADV_WILLNEED || advice == MADV_NORMAL)
    {
      if ((err = allocator_claim_pages(&allocator->base, addr, num_pages)) == NULL)
	err = allocator_map_pages(&allocator->base, addr, num_pages, 0);
    }
  else
    err = armeabisupport_bad_param;

  return err;
}

void mmap_dump(void)
{
  allocator_dump_all(&global.mmap_allocator_pool);
}

/* Remove any mmap allocations from the mmap pool for the given app.  */
void mmap_cleanup_app(app_object *app)
{
  abort_deregister(get_mmap_abort_handler(), (unsigned)&global.mmap_allocator_pool);

  armeabisupport_allocator_mmap *allocator = (armeabisupport_allocator_mmap *)linklist_first_armeabisupport_allocator(&global.mmap_allocator_pool);
  while (allocator)
    {
      armeabisupport_allocator_mmap *next = (armeabisupport_allocator_mmap *)linklist_next_armeabisupport_allocator(&allocator->base);
      allocator_mmap_cleanup_app (allocator, app);
      if (allocator->block_list.count == 0)
        {
          TRACE("mmap_cleanup_app: Allocator %s empty, destroying to reclaim dynamic area", allocator->base.name);
          /* No more allocations in this allocator, remove from the pool.  */
          linklist_remove(&global.mmap_allocator_pool, &allocator->base.link);
          allocator_destroy_internal(&allocator->base);
        }
      allocator = next;
    }
}

_kernel_oserror *
armeabi_mprotect(void *addr, size_t len, int prot)
{
  TRACE("armeabi_mprotect(%p, 0x%X, %d)", addr, len, prot);

  /* The caller should already have checked this.  */
  if (!is_page_aligned(addr) || len == 0)
    return armeabisupport_bad_param;

  armeabisupport_allocator_mmap *allocator = get_allocator(addr);
  if (!allocator)
    {
      LOG("armeabi_mprotect: Failed to find address in allocator pool");
      return armeabisupport_bad_param;
    }

  uint32_t access_flags;
  _kernel_oserror *err = convert_protection(prot, &access_flags);
  if (err)
    return err;

  uint32_t num_pages = bytes_to_pages(len);
  return allocator_change_protection(allocator, addr, num_pages, access_flags);
}

static _kernel_oserror *
armeabi_get_info(eabi_PTR addr,
		 eabi_PTR *mmap_addr,
		 size_t *size,
		 int *prot,
		 int *flags,
		 int *handle,
		 size_t *offset)
{
  if (!is_page_aligned(addr))
    return armeabisupport_bad_param;

  armeabisupport_allocator_mmap *allocator = get_allocator(addr);
  if (!allocator)
    {
      LOG("armeabi_get_info: Failed to find address in allocator pool");
      return armeabisupport_bad_param;
    }

  mmap_block *block = get_block(allocator, addr);
  if (!block)
    return armeabisupport_bad_param;

  *mmap_addr = page_to_addr(&allocator->base, block->start_page);
  *size = pages_to_bytes(block->end_page - block->start_page);
  *prot = block->prot;
  *flags = block->flags;
  *handle = block->fd;
  *offset = block->offset;

  TRACE("armeabi_get_info: addr=%p, mmap_addr=%p size=0x%X, prot=0x%X flags=0x%X, fd=%d, offset=%d",
	addr, *mmap_addr, *size, *prot, *flags, *handle, *offset);

  return NULL;
}

_kernel_oserror *
armeabi_mremap (eabi_PTR addr,
		size_t old_len,
		size_t new_len,
		int flags,
		armeabisupport_allocator_mmap **allocator_ret,
		mmap_block **block_ret)
{
  TRACE("armeabi_mremap: addr=%p, old=%X, new=%X, flags=%X",
	addr, old_len, new_len, flags);

  _kernel_oserror *err;

  if (new_len == 0)
    {
      LOG("armeabi_mremap: new_len = 0");
      err = armeabisupport_bad_param;
      goto error;
    }

  armeabisupport_allocator_mmap *allocator = get_allocator(addr);
  if (!allocator)
    {
      LOG("armeabi_mremap: Failed to find address (%p) in mmap allocator pool", addr);
      err = armeabisupport_bad_param;
      goto error;
    }

  mmap_block *block = get_block(allocator, addr);
  if (!block)
    {
      LOG("armeabi_mremap: Failed to find block");
      err = armeabisupport_bad_param;
      goto error;
    }

  if (new_len == old_len)
    {
      *allocator_ret = allocator;
      *block_ret = block;
      return NULL;
    }

  /* We can only remap a block that was fully allocated previously.  */
  int page = get_page_number(&allocator->base, addr);
  if (page != block->start_page || bytes_to_pages(old_len) != block->end_page - block->start_page)
    {
      LOG("armeabi_mremap: Non matching block, looking for (%d : %d), got (%d : %d)",
	  page, bytes_to_pages(old_len), block->start_page, block->end_page - block->start_page);
      err = armeabisupport_bad_param;
      goto error;
    }

  /* Ok, we have a block that exactly matches the one in the remap request.  */
  return allocator_mremap(allocator,
			  block,
			  bytes_to_pages(new_len - old_len),
			  flags,
			  allocator_ret,
			  block_ret);
error:
  *allocator_ret = NULL;
  *block_ret = NULL;

  return err;
}

_kernel_oserror *
armeabi_get_page_protection (eabi_PTR addr, int *prot_ret)
{
  armeabisupport_allocator_mmap *allocator = get_allocator(addr);
  int page = get_page_number(&allocator->base, addr);

  pmp_page_info_entry entry;
  entry.pmp_page_index = page;
  entry.physical_page_number = -1;
  entry.da_page_number = -1;
  entry.flags = 0;
  _kernel_oserror *err = dynamic_pmp_page_info(allocator->base.da.number, &entry, 1);
  if (err || entry.da_page_number == -1)
    {
      LOG("allocator_change_protection: dynamic_pmp_page_info error: %s", err->errmess);
      return armeabisupport_page_map_error;
    }

  int flags = get_access_permissions(entry.flags);
  int prot = PROT_NONE;
  if (flags != -1)
    {
      if (flags & OS_PERMISSION_FLAG_USER_EXEC)
        prot |= PROT_EXEC;
      if (flags & OS_PERMISSION_FLAG_USER_WRITE)
        prot |= PROT_WRITE;
      if (flags & OS_PERMISSION_FLAG_USER_READ)
        prot |= PROT_READ;
    }

  *prot_ret = prot;

  return NULL;
}
