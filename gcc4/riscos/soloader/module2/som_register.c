/* som_register.c
 *
 * Copyright 2007, 2009 GCCSDK Developers
 * Written by Lee Noar
 */

#include <kernel.h>
#include <string.h>
#include "som_register.h"
#include "som.h"
#include "som_alloc.h"
#include "som_workspace.h"
#include "som_array.h"
#include "som_history.h"

static _kernel_oserror *
init_object (som_object *object, const som_objinfo *objinfo)
{
  object->base_addr = objinfo->base_addr;
  object->rw_addr = object->private_rw_ptr = objinfo->public_rw_ptr;
  object->rw_size = objinfo->rw_size;
  object->end_addr = object->rw_addr + object->rw_size;
  object->got_addr = object->private_got_ptr =
    object->rw_addr + objinfo->got_offset;
  object->bss_addr = object->rw_addr + objinfo->bss_offset;
  object->bss_size = objinfo->bss_size;
  object->dynamic_addr = object->rw_addr + objinfo->dyn_offset;
  object->dynamic_size = objinfo->dyn_size;
  object->flags = objinfo->flags;
  object->usage_count = 0;
  object->expire_time = 0;
  object->index = 0;
  if (objinfo->name)
    {
      _kernel_oserror *err;

      if ((err = som_alloc (strlen (objinfo->name) + 1,
			    (void **) (void *) &object->name)) != NULL)
	return err;
      strcpy (object->name, objinfo->name);
    }

  return NULL;
}

static void
copy_object (som_object *new_obj, som_object *old_obj)
{
  new_obj->handle = old_obj->handle;
  new_obj->base_addr = old_obj->base_addr;
  new_obj->end_addr = old_obj->end_addr;
  new_obj->rw_addr = old_obj->rw_addr;
  new_obj->rw_size = old_obj->rw_size;
  new_obj->got_addr = old_obj->got_addr;
  new_obj->bss_addr = old_obj->bss_addr;
  new_obj->bss_size = old_obj->bss_size;
  new_obj->dynamic_addr = old_obj->dynamic_addr;
  new_obj->dynamic_size = old_obj->dynamic_size;
  new_obj->name = old_obj->name;
  new_obj->flags = old_obj->flags;
  new_obj->index = old_obj->index;

  new_obj->usage_count = 0;
  new_obj->expire_time = 0;
  new_obj->private_rw_ptr = 0;
  new_obj->private_got_ptr = 0;

  linklist_init_link (&new_obj->link);
}

/* Add a shared object to a linked list so that the list is ordered by
   increasing load address of library.  */
static void
som_add_sharedobject (link_list *list, som_object *insert_obj)
{
  som_object *list_obj;

  for (list_obj = linklist_first_som_object (list);
       list_obj != NULL && insert_obj->base_addr >= list_obj->base_addr;
       list_obj = linklist_next_som_object (list_obj))
    /* */;

  /* This catches the cases where the list is empty or the object should
     be linked to the end of the list.  */
  if (list_obj == NULL)
    linklist_add_to_end (list, &insert_obj->link);
  else
    linklist_add_before (list, &list_obj->link, &insert_obj->link);
}

_kernel_oserror *
som_register_client (som_handle handle, som_objinfo *objinfo)
{
  _kernel_oserror *err;
  som_client *client = NULL;
  som_object *object = NULL;
  unsigned int ID;

  if ((err = som_alloc (sizeof (som_client),
			(void **) (void *) &client)) != NULL)
    goto error;

  /* Find a unique ID that hasn't already been used. There is a danger that
     if multiple tasks are run in quick succession, they may get the same
     centisecond count (and therefore ID) from OS_ReadMonotonicTime. Make
     sure this doesn't happen.
     We do this here before the call to linklist_add_tail(), so that the
     new client is not considered by find_client().  */
  ID = os_read_monotonic_time ();
  while (true)
    {
      som_client *test = som_find_client (ID);	/* Should return 0 if the new ID is unique.  */

      if (test == NULL)
	break;

      ID++;
    }

  client->unique_ID = ID;

  rt_workspace_set (rt_workspace_CLIENT_STRUCT_PTR, (unsigned int) client);

  linklist_init_list (&client->object_list);

  if ((err = som_alloc (strlen (objinfo->name) + 1,
			(void **) (void *) &client->name)) != NULL)
    goto error;
  strcpy (client->name, objinfo->name);

  /* Zero the object name for the client list object to stop init_object
     from making a copy. Rather than have the name copied twice, once for
     the client and once for the client object, we'll use the same copy
     for both.  */
  objinfo->name = NULL;

  /* Allocate an object struct to record the details of the client in its
     object list. This object is not in the global list - only library
     objects exist in the global list.  */
  if ((err = som_alloc (sizeof (som_object),
		        (void **) (void *) &object)) != NULL)
    goto error;

  object->handle = handle;
  object->name = NULL;

  if ((err = init_object (object, objinfo)) != NULL)
    goto error;

  /* Use the same copy of the client's name for it and its object.  */
  object->name = client->name;

  object->flags.type = object_flag_type_CLIENT;

  linklist_add_to_end (&global.client_list, &client->link);

  /* The object list is ordered by base addr.  */
  som_add_sharedobject (&client->object_list, object);

  /* Store the object index in the GOT.  */
  *((unsigned int *) object->got_addr + SOM_OBJECT_INDEX_OFFSET) =
    object->index;

  /* Store the location of the client runtime array in the GOT.  */
  *((unsigned int *) object->got_addr + SOM_RUNTIME_ARRAY_OFFSET) =
    (unsigned int)rt_workspace_find();

  somarray_init (&client->runtime_array, sizeof (som_rt_elem), 0);

  return NULL;

error:
  if (client)
    {
      if (client->name)
	som_free (client->name);

      som_free (client);
    }

  if (object)
    {
      if (object->name)
	som_free (object->name);

      som_free (object);
    }

  return err;
}

/* Register a library for the current client. If the library is not already
   registered in the global object list, then an OBJECT_* structure is
   allocated and placed in the global list. Then regardless of whether the
   library was in the global list or not, its OBJECT_* structure is cloned
   and placed in the client's list.  */
_kernel_oserror *
som_register_sharedobject (som_handle handle, som_objinfo *objinfo,
			   som_object **object_ret)
{
  som_client *client;

  if ((client = FIND_CLIENT ()) == NULL)
    return somerr_client_not_found;

  som_object *obj;

  /* See if the library is already registered in the global list.  */
  for (obj = linklist_first_som_object (&global.object_list);
       obj != NULL && obj->handle != handle; /* Use the handle as the key.  */
       obj = linklist_next_som_object (obj))
    /* */;

  _kernel_oserror *err;
  som_object *client_obj = NULL;

  if (obj == NULL)
    {
      if ((err = som_alloc (sizeof (som_object),
			    (void **) (void *) &obj)) != NULL)
	goto error;

      if ((err = init_object (obj, objinfo)) != NULL)
	goto error;

      /* If it's new to the global list, then add to the array of
         objects.  */
      if ((err = somarray_add_object (&global.object_array, obj)) != NULL)
	  goto error;

      obj->handle = handle;

      som_add_sharedobject (&global.object_list, obj);
    }

  /* Make a copy of the object for the client's list.  */
  if ((err = som_alloc (sizeof (som_object),
			(void **) (void *) &client_obj)) != NULL)
    goto error;

  obj->usage_count++;

  copy_object (client_obj, obj);

  client_obj->private_rw_ptr = objinfo->private_rw_ptr;
  client_obj->private_got_ptr =
    client_obj->private_rw_ptr + objinfo->got_offset;

  som_add_sharedobject (&client->object_list, client_obj);

  if (object_ret)
    *object_ret = client_obj;

  return NULL;

error:
  if (obj)
    {
      if (obj->name)
	som_free (obj->name);
      som_free (obj);
    }

  if (client_obj)
    som_free (client_obj);

  return err;
}

/* SWI "SOM_RegisterObject"
 * entry:
 *  r0 = Reason code
 *  r1 = handle
 *  r2 = ptr to OBJINFO_* struct
 * exit:
 *  all other registers preserved if no error,
 *  otherwise r0 = ptr to error block
 */
_kernel_oserror *
som_register_object (_kernel_swi_regs *regs)
{
  unsigned int handle = (unsigned int) regs->r[1];
  som_objinfo *objinfo = (som_objinfo *) regs->r[2];
  _kernel_oserror *err = NULL;
  som_object *object = NULL;

  switch (regs->r[0])
    {
    case reason_code_SOM_REGISTER_LOADER:
      objinfo->flags.type = object_flag_type_LOADER;
      err = som_register_sharedobject (handle, objinfo, &object);
      break;

    case reason_code_SOM_REGISTER_CLIENT:
      err = som_register_client (handle, objinfo);
      break;

    case reason_code_SOM_REGISTER_LIBRARY:
      objinfo->flags.type = object_flag_type_SHARED;
      err = som_register_sharedobject (handle, objinfo, &object);
      break;
    }

  return err;
}

static _kernel_oserror *
deregister_shared_object (som_client *client,
			  unsigned int handle)
{
  /* First find the object in the client's list.  */
  som_object *client_obj;
  for (client_obj = linklist_first_som_object (&client->object_list);
       client_obj != NULL && client_obj->handle != handle;
       client_obj = linklist_next_som_object (client_obj))
    /* */;

  /* Next find the object in the global list.  */
  som_object *global_obj;
  for (global_obj = linklist_first_som_object (&global.object_list);
       global_obj != NULL && global_obj->handle != handle;
       global_obj = linklist_next_som_object (global_obj))
    /* */;

  if (global_obj == NULL)
    return somerr_object_not_found;

  /* If the object was linked to the client, then it is marked for expiry.
     If the object is not used by any client, then assume that this is a
     specific request to remove it from the system immediately.  */
  if (client_obj)
    {
      /* Remove object from client list.  */
      linklist_remove (&client->object_list, &client_obj->link);

      /* Free memory used to store object in client list.  */
      som_free (client_obj);

      /* Now deal with global object.
         If the usage count is not zero, then another client is using the
         library and it should not be marked for expiry.  */
      if (--global_obj->usage_count)
        return NULL;

      /* Usage count is zero, so object is no longer in use at all. We
         don't remove it from memory straight away, but mark it for
         expiry.  */
      global_obj->expire_time = os_read_monotonic_time () + global.max_idle_time;
    }
  else if (global_obj->usage_count == 0)
    {
      /* Remove from the global list.  */
      linklist_remove (&global.object_list, &global_obj->link);

      /* Mark this slot in the array as re-usable.  */
      global.object_array.object_base[global_obj->index] = NULL;

      som_free (global_obj->name);
      som_free (global_obj->base_addr);
      som_free (global_obj);
    }
  /*
  else
    In theory, there shouldn't be another client using the object (i.e.,
    usage_count > 0). If the dynamic loader is trying to remove a bogus
    library, then it must be for the first client that tried to use it.
    However, we will make no assumptions. If the usage count is not 0,
    then leave it alone.  */

  return NULL;
}

_kernel_oserror *
som_deregister_client (void)
{
  som_client *client;

  if ((client = FIND_CLIENT ()) == NULL)
    return somerr_unknown_client;

  rt_workspace_set (rt_workspace_CLIENT_STRUCT_PTR, 0);

  /* From now on, FIND_CLIENT() will fail.  */

  som_history_add_client (client);

  /* The first link in the client's object list should be the application
     itself. This is treated differently as it is not a shared object as
     such (ie. it doesn't exist in the global list). This means that it
     should not be deregistered like normal objects.  */
  som_object *object = linklist_first_som_object (&client->object_list), *next_obj;

  if (object->flags.type == object_flag_type_CLIENT)
    {
      /* Get the next object before freeing the memory, otherwise we may
         get undefined behaviour.  */
      next_obj = linklist_next_som_object (object);

      /* Free the object that describes the client. Note that name is
         borrowed from the client structure and so the memory is not freed
         here. It is freed below.  */
      som_free (object);

      object = next_obj;
    }

  /* Now loop through the rest of the objects in the list and deregister
     them.  */
  while (object)
    {
      next_obj = linklist_next_som_object (object);

      deregister_shared_object (client, object->handle);
      /* Ignore any errors from above.  */

      object = next_obj;
    }

  /* Unlink from the list of clients. */
  linklist_remove (&global.client_list, &client->link);

  somarray_fini (&client->runtime_array);

  if (client->name)
    som_free (client->name);

  som_free (client);

  return NULL;
}

_kernel_oserror *
som_deregister_shared_object (_kernel_swi_regs *regs)
{
  som_client *client;

  if ((client = FIND_CLIENT ()) == NULL)
    return somerr_unknown_client;

  return deregister_shared_object (client, regs->r[0]);
}
