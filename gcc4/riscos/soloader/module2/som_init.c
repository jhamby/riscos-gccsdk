/* som_init.c
 *
 * Copyright 2006, 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#include <stdlib.h>
#include <kernel.h>
#include <stdio.h>
#include "somanager.h"
#include "som.h"
#include "swis.h"
#include "som_os_swis.h"
#include "som_alloc.h"
#include "som_array.h"

static bool is_host_32bit(void)
{
bool res;

  asm volatile ("TEQ	PC, PC;\n\t"
		"MOVNE	%0, #0;\n\t"
		"MOVEQ	%0, #1;\n\t"
		: "=r" (res)
		: /* no inputs */
		: "cc" /* Alters flags */);
  return res;
}

_kernel_oserror *
module_initialise(const char *tail, int podule_base, void *pw)
{
_kernel_oserror *err;

  global.data_da.number = global.library_da.number = -1;

  global.flags.host_32bit = is_host_32bit();

  global.max_idle_time = SOM_MAX_IDLE_TIME;

  global.call_every_cs_delay = SOM_CALL_EVERY_CS_DELAY;

  /* Create a dynamic area for general memory allocation */
  if ((err = dynamic_area_create("SO Manager", SOM_INIT_DA_SIZE, SOM_MAX_DA_SIZE, &global.data_da)) != NULL)
    goto error;

  /* Create a heap in the dynamic area */
  if ((err = heap_init(global.data_da.base_addr, SOM_INIT_DA_SIZE)) != NULL)
    goto error;

#ifdef LIBRARIES_IN_DA
  if (global.flags.host_32bit)
  {
    if ((err = dynamic_area_create("DSO Libraries", SOM_INIT_LIBDA_SIZE, SOM_MAX_LIBDA_SIZE, &global.library_da)) != NULL)
      goto error;

    if ((err = heap_init(global.library_da.base_addr, SOM_INIT_LIBDA_SIZE)) != NULL)
      goto error;
  }
#endif

  if ((err = somarray_init(&global.object_array, sizeof(som_object *), 1)) != NULL)
    goto error;

  som_start_call_every(pw);

  return NULL;

error:
  if (global.data_da.number != -1)
    dynamic_area_remove(global.data_da.number);

#ifdef LIBRARIES_IN_DA
  if (global.library_da.number != -1)
    dynamic_area_remove(global.library_da.number);
#endif

  return err;
}
