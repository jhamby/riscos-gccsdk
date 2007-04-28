/* som_main.c
 *
 * Copyright 2007 GCCSDK Developers
 */

#include "som.h"
#include "link_list.h"
#include "som_alloc.h"

som_globals	global;

/* Return the client object whose ID is given. */
som_client *som_find_client(som_handle ID)
{
som_client *client;

  client = linklist_first_som_client(&global.client_list);
  while (client)
  {
    if (client->unique_ID == ID)
    {
      global.cached_client_ptr = client;
      global.cached_client_ID = ID;

      return client;
    }

    client = linklist_next_som_client(client);
  }

  return NULL;
}

/* SWI "SOM_Prologue"
 * entry:
 *  r8 = PLT address
 * exit:
 *  r8 = private GOT ptr
 */
_kernel_oserror *som_prologue(_kernel_swi_regs *regs)
{
  /* Fast case, check the cached values. */
  if (rt_workspace_get(rt_workspace_CLIENT_ID) == global.cached_client_ID)
  {
    if (rt_workspace_get(rt_workspace_CACHED_PLT) == regs->r[8])
    {
      regs->r[8] = rt_workspace_get(rt_workspace_CACHED_PLTGOT);
      return NULL;
    }
  }

som_client *client = FIND_CLIENT();
  if (!client)
  {
    regs->r[8] = SOM_BAD_VALUE;
    return somerr_unknown_client;
  }

som_object *object = linklist_first_som_object(&client->object_list);
  while (object)
  {
    if (regs->r[8] <= (unsigned int)object->end_addr)
    {
      rt_workspace_set(rt_workspace_CACHED_PLT, regs->r[8]);
      regs->r[8] = (unsigned int)object->private_got_ptr;
      rt_workspace_set(rt_workspace_CACHED_PLTGOT, regs->r[8]);
      return NULL;
    }

    object = linklist_next_som_object(object);
  }

  regs->r[8] = SOM_BAD_VALUE;
  return somerr_object_not_found;
}

/* SWI "SOM_Resolver"
 * entry:
 *  r8 = &GOT[n+3] (pointer to function)
 * exit:
 *  r0 = &GOT[n+3] (pointer to function)
 *  r8 = &GOT[0]
 */
_kernel_oserror *som_resolver(_kernel_swi_regs *regs)
{
  regs->r[0] = regs->r[8];
  regs->r[8] = rt_workspace_get(rt_workspace_CACHED_PLTGOT);

  return NULL;
}

/* A callback handler that checks for unused objects that have expired and
   which can be removed from memory. */
_kernel_oserror *som_callback_handler(_kernel_swi_regs *r, void *pw)
{
  global.flags.callback_pending = false;

unsigned int current_time = os_read_monotonic_time();
som_object *object = linklist_first_som_object(&global.object_list);

  while (object)
  {
  som_object *next = linklist_next_som_object(object);

    /* Ignore libraries that are still in use. */
    if (object->usage_count != 0)
      goto next_object;

    /* Check object expiry time against current time. */
    if (object->expire_time > current_time)
      goto next_object;

    /* If we get here, object has expired, remove from global list. */
    linklist_remove(&global.object_list, &object->link);

    som_free(object->name);
    som_free(object->base_addr);
    som_free(object);

next_object:
    object = next;
  }

  return NULL;
}

/* A handler that is called regularly to set the (above) callback. */
_kernel_oserror *som_call_every_handler(_kernel_swi_regs *r, void *pw)
{
  os_add_callback(som_callback, pw);

  global.flags.callback_pending = true;

  return NULL;
}

/* Start a ticker going to check for unused, expired objects, a bit like garbage
   collection. Called once at module initialisation. */
_kernel_oserror *som_start_call_every(void *pw)
{
  if (global.flags.call_every_enabled)
    return NULL;

_kernel_oserror *err = os_call_every(global.call_every_cs_delay, som_call_every, pw);

  if (!err)
    global.flags.call_every_enabled = true;

  return err;
}

/* Stop the garbage collection ticker. Called once at module finalisation. */
_kernel_oserror *som_stop_call_every(void *pw)
{
  if (!global.flags.call_every_enabled)
    return NULL;

_kernel_oserror *err = os_remove_ticker_event(som_call_every, pw);

  if (!err)
    global.flags.call_every_enabled = false;

  /* Ensure a callback doesn't occur after the module has gone. */
  if (global.flags.callback_pending)
    os_remove_callback(som_callback, pw);

  return err;
}
