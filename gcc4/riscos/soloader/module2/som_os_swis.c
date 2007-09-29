/* som_os_swis.c
 *
 * Copyright 2007 GCCSDK Developers
 */

#include <stdlib.h>
#include "som_os_swis.h"

_kernel_oserror *
dynamic_area_create (const char *name, int init_size, int max_size,
		     dynamic_area_block *ret)
{
  _kernel_oserror *err;
  _kernel_swi_regs regs;

  /* Create a dynamic area for general memory allocation.  */
  regs.r[0] = reason_code_DYNAMIC_AREA_CREATE;
  regs.r[1] = -1;
  regs.r[2] = init_size;
  regs.r[3] = -1;
  regs.r[4] = 0x80;		/* Not draggable */
  regs.r[5] = max_size;
  regs.r[6] = 0;
  regs.r[7] = 0;
  regs.r[8] = (int) name;
  if ((err = _kernel_swi (XOS_Bit | OS_DynamicArea, &regs, &regs)) != NULL)
    return err;

  if (ret)
    {
      ret->number = regs.r[1];
      ret->base_addr = (void *) regs.r[3];
      ret->max_size = regs.r[5];
    }

  return NULL;
}
