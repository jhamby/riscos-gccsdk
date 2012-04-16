/*
 * riscos-memory.c
 * 
 * Author:
 * 	Lee Noar
 *
 * Memory allocator using a dynamic area for large blocks of memory.
 */

#include <kernel.h>
#include <swis.h>
#include <stdlib.h>
#include <glib.h>

#define MAX_DA_SIZE (200 * 1024 * 1024)
#define INIT_DA_SIZE (4 * 1024)
#define ALIGNMENT 8

/* Must be a multiple of ALIGNMENT in size.  */
typedef struct block_header
{
  size_t size;
  void *start;
} block_header;

typedef struct dynamic_area
{
  int handle;
  void *base_addr;
} dynamic_area;

static const char da_name[] = "Mono";
static dynamic_area mono_da = { -1, NULL };

static void
dynamic_area_remove (dynamic_area *da)
{
	_kernel_swi_regs regs;

	regs.r[0] = 1;
	regs.r[1] = da->handle;
	_kernel_swi (OS_DynamicArea, &regs, &regs);
}

static _kernel_oserror *
heap_claim (void *base_addr, size_t size, void **block_ret)
{
	_kernel_swi_regs regs;
	_kernel_oserror *err;

	regs.r[0] = 2;
	regs.r[1] = (int)base_addr;
	regs.r[3] = size;
	*block_ret = ((err = _kernel_swi (OS_Heap, &regs, &regs)) == NULL) ? (void *)regs.r[2] : NULL;

	return err;
}

static _kernel_oserror *
heap_extend (void *base_addr, size_t by)
{
	_kernel_swi_regs regs;

	regs.r[0] = 5;
	regs.r[1] = (int)base_addr;
	regs.r[3] = by;
	return _kernel_swi (OS_Heap, &regs, &regs);
}

static _kernel_oserror *
heap_release (void *base_addr, void *block)
{
	_kernel_swi_regs regs;

	regs.r[0] = 3;
	regs.r[1] = (int)base_addr;
	regs.r[2] = (int)block;
	return _kernel_swi (OS_Heap, &regs, &regs);
}

static _kernel_oserror *
dynamic_area_extend (int handle, size_t by)
{
	_kernel_swi_regs regs;

	regs.r[0] = handle;
	regs.r[1] = by;
	return _kernel_swi (OS_ChangeDynamicArea, &regs, &regs);
}

static void
mono_riscos_memory_allocator_fini (void)
{
  dynamic_area_remove (&mono_da);
}

static int
mono_riscos_memory_allocator_init (void)
{
	_kernel_swi_regs regs;

	/* Create the dynamic area.  */
	regs.r[0] = 0;
	regs.r[1] = -1;
	regs.r[2] = INIT_DA_SIZE;
	regs.r[3] = -1;
	regs.r[4] = 0x80;
	regs.r[5] = MAX_DA_SIZE;
	regs.r[6] = 0;
	regs.r[7] = 0;
	regs.r[8] = (int)da_name;
	if (_kernel_swi (OS_DynamicArea, &regs, &regs))
		return -1;

	mono_da.handle = regs.r[1];
	mono_da.base_addr = (void *)regs.r[3];

	/* Create a heap in the dynamic area.  */
	regs.r[0] = 0;
	regs.r[1] = (int)mono_da.base_addr;
	regs.r[3] = INIT_DA_SIZE;
	if (_kernel_swi (OS_Heap, &regs, &regs) == NULL) {
		atexit (mono_riscos_memory_allocator_fini);
		return 0;
	}

	if (mono_da.handle != -1)
		dynamic_area_remove (&mono_da);

	return -1;
}

/* mono_riscos_memory_alloc
 * 
 * Returns memory block aligned to a multiple of 8 bytes.
 * Uses a small header to record the real start address so that
 * it can be freed correctly.
 */
void *
mono_riscos_memory_alloc (size_t size)
{
	gboolean heap_extended = FALSE;
	void *block;
	block_header *hdr;

	/* Allow enough for the header and any alignment that may be required.  */
	size_t allocated_size = size + sizeof (block_header) + ALIGNMENT;

	/* Initialise on first use.  */
	if (mono_da.handle < 0)
		if (mono_riscos_memory_allocator_init () < 0)
			return NULL;

	/* If an initial request to allocate a block fails, extend the dynamic
	 * area and heap and try again. If it stills fails, then return NULL
	 * to indicate failure.  */
	while (TRUE) {
		_kernel_oserror *err = heap_claim (mono_da.base_addr, size, &block);
		if (err || block == NULL) {
			size_t heap_inc;

			if (heap_extended)
				return NULL;

			heap_inc = (((size * 2) + 0xfff) & ~0xfff);
			if ((err = dynamic_area_extend (mono_da.handle, heap_inc)) != NULL)
				return NULL;

			if ((err = heap_extend (mono_da.base_addr, heap_inc)) != NULL)
				return NULL;

			heap_extended = TRUE;
		} else {
			break;
		}
	}

	hdr = (block_header *)(((unsigned int)block + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1));
	hdr->start = block;
	hdr->size = allocated_size;

	return hdr + 1;
}

void
mono_riscos_memory_free (void *block)
{
	block_header *hdr = ((block_header *)block) - 1;

	block = hdr->start;

	heap_release (mono_da.base_addr, block);
}
