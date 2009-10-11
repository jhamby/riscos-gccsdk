/* som_init.c
 *
 * Copyright 2006, 2007, 2009 GCCSDK Developers
 * Written by Lee Noar
 */

#include <stdlib.h>
#include <kernel.h>
#include <stdio.h>
#include "somanager.h"
#include "som.h"
#include "swis.h"
#include "som_os_swis.h"
#include "som_array.h"

/* This routines assumes that it is run in a non-USR mode.  */
static bool
is_host_32bit (void)
{
  bool is32bit;

  asm volatile ("SUBS	%[is32bit], r0, r0\n\t" /* Set at least one status flag. */
		"TEQ	pc, pc\n\t"
		"MOVEQ	%[is32bit], #1\n\t"
		: [is32bit] "=r" (is32bit)
		: /* no inputs */
		: "cc");

  return is32bit;
}

_kernel_oserror *
module_initialise (const char *tail, int podule_base, void *pw)
{
  _kernel_oserror *err;

  /* Initialise all our global variables united in the 'global' som_globals
     structure.  */
  memset (&global, 0, sizeof (som_globals));
  global.data_da.number = global.library_da.number = -1;
  // linklist_init_list (&global.client_list);
  // linklist_init_list (&global.object_list);
  // linklist_init_list (&global.client_history);
  // global.object_array gets initialised further on.
  global.max_idle_time = SOM_MAX_IDLE_TIME;
  global.call_every_cs_delay = SOM_CALL_EVERY_CS_DELAY;
  // global.flags.no_client_check = false;
  global.flags.host_32bit = is_host_32bit ();
  // global.flags.callback_pending = false;
  // global.flags.call_every_enabled = false;

  /* Create a dynamic area for general memory allocation.  */
  if ((err = dynamic_area_create ("SO Manager", SOM_INIT_DA_SIZE,
				  SOM_MAX_DA_SIZE,
				  &global.data_da)) != NULL)
    goto error;

  /* Create a heap in the dynamic area.  */
  if ((err = heap_init (global.data_da.base_addr, SOM_INIT_DA_SIZE)) != NULL)
    goto error;

#ifdef LIBRARIES_IN_DA
  if (global.flags.host_32bit)
    {
      if ((err = dynamic_area_create ("Shared Libraries", SOM_INIT_LIBDA_SIZE,
				      SOM_MAX_LIBDA_SIZE,
				      &global.library_da)) != NULL)
	goto error;

      if ((err = heap_init (global.library_da.base_addr,
			    SOM_INIT_LIBDA_SIZE)) != NULL)
	goto error;
    }
#endif

  if ((err = somarray_init (&global.object_array,
			    sizeof (som_object *), 1)) != NULL)
    goto error;

  if ((err = som_start_call_every (pw)) != NULL)
    goto error;

  return NULL;

error:
  if (global.data_da.number != -1)
    dynamic_area_remove (global.data_da.number);

#ifdef LIBRARIES_IN_DA
  if (global.library_da.number != -1)
    dynamic_area_remove (global.library_da.number);
#endif

  return err;
}
