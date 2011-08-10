/* som_main.c
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#include "som.h"
#include "link_list.h"
#include "som_alloc.h"

som_globals global;

/* Return the client object whose ID is given.  */
som_client *
som_find_client (som_handle ID)
{
  som_client *client;

  for (client = linklist_first_som_client (&global.client_list);
       client != NULL && client->unique_ID != ID;
       client = linklist_next_som_client (client))
    /* */;

  return client;
}

/* A callback handler that checks for unused objects that have expired and
   which can be removed from memory.  */
_kernel_oserror *
som_callback_handler (_kernel_swi_regs *r, void *pw)
{
  global.flags.callback_pending = false;

  unsigned int current_time = os_read_monotonic_time ();
  som_library_object *global_library =
		linklist_first_som_library_object (&global.object_list);

  while (global_library)
    {
      som_library_object *next = linklist_next_som_library_object (global_library);

      /* Ignore libraries that are still in use.  */
      if (global_library->usage_count != 0)
	goto next_object;

      /* Check object expiry time against current time.  */
      if (global_library->expire_time > current_time)
	goto next_object;

      /* If we get here, object has expired, remove from global list.  */
      linklist_remove (&global.object_list, &global_library->link);

      /* Mark this object slot as being reusable.  */
      global.object_array.object_base[global_library->object.index] = NULL;

      som_free (global_library->object.base_addr);
      som_free (global_library);

    next_object:
      global_library = next;
    }

  return NULL;
}

/* A handler that is called regularly to set the (above) callback.  */
_kernel_oserror *
som_call_every_handler (_kernel_swi_regs *r, void *pw)
{
  os_add_callback (som_callback, pw);

  global.flags.callback_pending = true;

  return NULL;
}

/* Start a ticker going to check for unused, expired objects, a bit like
   garbage collection. Called once at module initialisation.  */
_kernel_oserror *
som_start_call_every (void *pw)
{
  if (global.flags.call_every_enabled)
    return NULL;

  _kernel_oserror *err;
  if ((err = os_call_every (global.call_every_cs_delay,
			    som_call_every, pw)) == NULL)
    global.flags.call_every_enabled = true;

  return err;
}

/* Stop the garbage collection ticker. Called once at module
   finalisation.  */
_kernel_oserror *
som_stop_call_every (void *pw)
{
  if (!global.flags.call_every_enabled)
    return NULL;

  _kernel_oserror *err;
  if ((err = os_remove_ticker_event (som_call_every, pw)) == NULL)
    global.flags.call_every_enabled = false;

  /* Ensure a callback doesn't occur after the module has gone.  */
  if (global.flags.callback_pending)
    os_remove_callback (som_callback, pw);

  return err;
}
