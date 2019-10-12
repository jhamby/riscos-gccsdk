/* swi.c
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#include <stdlib.h>
#include <kernel.h>
#include "memory.h"
#include "swi.h"

#define DYNAMICAREA_FLAG_REQUIRES_SPECIFIC_PAGES (1 << 8)
#define DYNAMICAREA_FLAG_IS_PMP (1 << 20)

extern _kernel_oserror *da_handler(void);

_kernel_oserror *
dynamic_area_create (const char *name, int max_size,
		     unsigned type,
		     dynamicarea_block *ret)
{
  _kernel_oserror *err;
  _kernel_swi_regs regs;
  unsigned initial_size = 0;

  /* Create a dynamic area for general memory allocation.  */
  regs.r[0] = 0;		/* reason code - create */
  regs.r[1] = -1;
  regs.r[3] = -1;
  regs.r[4] = 0x80;		/* Not draggable */
  regs.r[5] = max_size;
  regs.r[8] = (int) name;
  if (type == ALLOCATOR_TYPE_PAGE)
    {
      /* PMP dynamic areas must have a logical initial size of 0. */
      regs.r[2] = 0;
      regs.r[4] |= (DYNAMICAREA_FLAG_IS_PMP | DYNAMICAREA_FLAG_REQUIRES_SPECIFIC_PAGES);
      regs.r[6] = (int)da_handler;
      regs.r[7] = -1;
      regs.r[9] = bytes_to_pages (max_size);
    }
  else
    {

      if (type == ALLOCATOR_TYPE_SINGLE_USE)
	initial_size = max_size;
      else if (type == ALLOCATOR_TYPE_HEAP)
	initial_size = ALLOCATOR_HEAP_INITIAL_SIZE; /* Need 1 page initial size to hold heap descriptor (which is only 8 bytes).  */

      regs.r[2] = (int)initial_size;
      regs.r[6] = 0;
      regs.r[7] = 0;
    }

  err = _kernel_swi (XOS_Bit | OS_DynamicArea, &regs, &regs);

  if (!err && ret)
    {
      ret->number = regs.r[1];
      ret->base = (eabi_PTR)regs.r[3];
      ret->end = (eabi_PTR)regs.r[3] + initial_size;
    }

  return err;
}
