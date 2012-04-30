/* som_register.c
 *
 * Copyright 2007-2011 GCCSDK Developers
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

/* The current version of the .riscos.pic section that we are aware of.  */
#define SRELPIC_VERSION 1

/* Structure defining the lay out of the ".riscos.pic" section.  */
typedef struct srelpic_section
{
  unsigned int version;
  unsigned int rel_count;
  struct rel
  {
    /* Offset of the reloc from the start of the library.  */
    unsigned int offset : 30;

    unsigned int reserved : 1;

    /* Set for an __GOTT_INDEX__ reloc, clear for a __GOTT_BASE__ reloc.  */
    unsigned int index : 1;
  } array[];
} srelpic_section;

static _kernel_oserror *
init_object (som_object *object, const som_objinfo *objinfo)
{
  object->base_addr = objinfo->base_addr;
  object->rw_addr = objinfo->public_rw_ptr;
  object->rw_size = objinfo->rw_size;
  object->end_addr = object->rw_addr + object->rw_size - objinfo->bss_size;
  object->got_addr = object->rw_addr + objinfo->got_offset;
  object->bss_addr = object->rw_addr + objinfo->bss_offset;
  object->bss_size = objinfo->bss_size;
  object->dynamic_addr = object->rw_addr + objinfo->dyn_offset;
  object->dynamic_size = objinfo->dyn_size;
  object->flags = objinfo->flags;
  object->index = 0;

  strcpy (object->name, objinfo->name);

  return NULL;
}

/* Add a shared object to a linked list so that the list is ordered by
   increasing load address of library.  */
static void
som_add_global_library (link_list *list,
			som_library_object *insert)
{
  som_library_object *global_library;

  for (global_library = linklist_first_som_library_object (list);
       global_library != NULL && insert->object.base_addr >= global_library->object.base_addr;
       global_library = linklist_next_som_library_object (global_library))
    /* */;

  /* This catches the cases where the list is empty or the object should
     be linked to the end of the list.  */
  if (global_library == NULL)
    linklist_add_to_end (list, &insert->link);
  else
    linklist_add_before (list, &global_library->link, &insert->link);
}

static void
som_add_client_library (link_list *list,
			som_client_object *insert)
{
  som_client_object *client_library;

  for (client_library = linklist_first_som_client_object (list);
       client_library != NULL
	 && insert->library->object.base_addr >= client_library->library->object.base_addr;
       client_library = linklist_next_som_client_object (client_library))
    /* */;

  /* This catches the cases where the list is empty or the object should
     be linked to the end of the list.  */
  if (client_library == NULL)
    linklist_add_to_end (list, &insert->link);
  else
    linklist_add_before (list, &client_library->link, &insert->link);
}

_kernel_oserror *
som_register_client (som_handle handle,
		     som_objinfo *objinfo)
{
  _kernel_oserror *err;
  som_client *client = NULL;
  unsigned int ID;

  if ((err = som_alloc (sizeof (som_client) + strlen (objinfo->name) + 1,
			(void **) (void *) &client)) != NULL)
    return err;
  memset (client, 0, sizeof (som_client));

  /* Find a unique ID that hasn't already been used. There is a danger that
     if multiple tasks are run in quick succession, they may get the same
     centisecond count (and therefor ID) from OS_ReadMonotonicTime. Make
     sure this doesn't happen.
     We do this here before the call to linklist_add_to_end(), so that the
     new client is not considered by find_client().  */
  ID = os_read_monotonic_time ();
  while (true)
    {
      som_client *test = som_find_client (ID);	/* Should return 0 if the new ID is unique.  */

      if (test == NULL)
	break;

      ID++;
    }

  rt_workspace_set (rt_workspace_CLIENT_STRUCT_PTR, (unsigned int) client);

  client->unique_ID = ID;
  client->object.handle = handle;
  client->object.flags.type = object_flag_type_CLIENT;

  init_object (&client->object, objinfo);

  linklist_add_to_end (&global.client_list, &client->link);

  /* Store the object index in the runtime workspace.  */
  rt_workspace_set (rt_workspace_OBJECT_INDEX, 0);

  if ((err = somarray_init (&client->runtime_array, sizeof (som_rt_elem), 0)) != NULL
   || (err = somarray_init (&client->gott_base, sizeof (void *), 0)) != NULL)
    goto cleanup;

  /* A GOT with a 5 word header was built with GCC 4.1.*. A 3 word header
     won't have a zero at index 3.  */
  if (*((unsigned int *) client->object.got_addr + 3) == 0)
    {
      /* This is already zero:
      *((unsigned int *) client->got_addr + SOM_OBJECT_INDEX_OFFSET) = 0;  */
      *((unsigned int *) client->object.got_addr + SOM_RUNTIME_ARRAY_OFFSET) = RT_WORKSPACE_RUNTIME_ARRAY;
    }

  return NULL;

cleanup:
  /* We know client != NULL here.  */
  somarray_fini (&client->runtime_array);
  somarray_fini (&client->gott_base);
  som_free (client);

  return err;
}

static _kernel_oserror *
process_relpic (som_object *object)
{
  const Elf32_Dyn *dyn;
  for (dyn = (const Elf32_Dyn *) object->dynamic_addr;
       dyn->d_tag != DT_NULL && dyn->d_tag != DT_RISCOS_PIC;
       dyn++)
    /* Empty loop. */;

  if (dyn->d_tag == DT_NULL)
    return somerr_srelpic_unknown;

  srelpic_section *srelpic = (srelpic_section *)(dyn->d_un.d_ptr + object->base_addr);

  if (srelpic->version > SRELPIC_VERSION)
    return somerr_srelpic_unknown; /* We don't know how to deal with this.  */

  for (int i = 0;
       i < srelpic->rel_count;
       i++)
    {
      unsigned int *location = (unsigned int *)(object->base_addr +
				    srelpic->array[i].offset);
      if (srelpic->array[i].index)
        {
	  /* The elements of the array are 32 bits and the maximum offset the instruction
	     allows is 0xFFF. This gives a maximum of 1024 libraries loaded at any one time.  */
	  unsigned int gott_index = object->index * sizeof (void *);
	  if (gott_index >= 0x1000)
	    return somerr_srelpic_overflow;

	  /* Assume that the link editor has zero'd the lower 12 bits.  */
	  *location |= gott_index;
	}
      else
	*location = RT_WORKSPACE_GOTT_BASE;
    }

  os_synchronise_code_area (object->base_addr, object->rw_addr - 4);

  return NULL;
}

/* Register a library for the current client. Add the library to the global
   list if it's not already there.  */
_kernel_oserror *
som_register_sharedobject (som_handle handle,
			   som_objinfo *objinfo)
{
  som_client *client;

  if ((client = FIND_CLIENT ()) == NULL)
    return somerr_client_not_found;

  som_library_object *global_library;

  /* See if the library is already registered in the global list.  */
  for (global_library = linklist_first_som_library_object (&global.object_list);
       global_library != NULL && global_library->object.handle != handle; /* Use the handle as the key.  */
       global_library = linklist_next_som_library_object (global_library))
    /* */;

  _kernel_oserror *err;
  som_client_object *client_library = NULL;

  if (global_library == NULL)
    {
      if ((err = som_alloc (sizeof (som_library_object) + strlen (objinfo->name) + 1,
			    (void **) (void *) &global_library)) != NULL)
	return err;

      init_object (&global_library->object, objinfo);

      global_library->object.handle = handle;
      global_library->expire_time = 0;
      global_library->usage_count = 0;

      /* If it's new to the global list, then add to the object array...  */
      if ((err = somarray_add_object (&global.object_array,
				      &global_library->object)) != NULL)
        {
	  som_free (&global_library);
	  return err;
	}

      /* ... and to the list.  */
      som_add_global_library (&global.object_list, global_library);

      /* Process the section that contains __GOTT_INDEX__ and __GOTT_BASE__ relocations.  */
      if (process_relpic (&global_library->object) != NULL)
      {
	/* Store the object index in the library public GOT.  */
	((unsigned int *) global_library->object.got_addr)[SOM_OBJECT_INDEX_OFFSET] = global_library->object.index;

	/* Store the location of the client runtime array in the public GOT.  */
	((unsigned int *) global_library->object.got_addr)[SOM_RUNTIME_ARRAY_OFFSET] = RT_WORKSPACE_RUNTIME_ARRAY;
      }
    }

  /* Record the object in the client's list.
     If an error occurs here, then don't clean up the library. It may already be
     in use by another client. */
  if ((err = som_alloc (sizeof (som_client_object),
			(void **) (void *) &client_library)) != NULL)
    return err;

  global_library->usage_count++;

  client_library->rw_addr = objinfo->private_rw_ptr;
  client_library->got_addr = client_library->rw_addr + objinfo->got_offset;
  client_library->library = global_library;

  som_add_client_library (&client->object_list, client_library);

  return NULL;
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

  switch (regs->r[0])
    {
    case reason_code_SOM_REGISTER_LOADER:
      objinfo->flags.type = object_flag_type_LOADER;
      err = som_register_sharedobject (handle, objinfo);
      break;

    case reason_code_SOM_REGISTER_CLIENT:
      err = som_register_client (handle, objinfo);
      break;

    case reason_code_SOM_REGISTER_LIBRARY:
      objinfo->flags.type = object_flag_type_SHARED;
      err = som_register_sharedobject (handle, objinfo);
      break;
    }

  return err;
}

static _kernel_oserror *
deregister_shared_object (som_client *client,
			  unsigned int handle)
{
  /* First find the object in the client's list.  */
  som_client_object *client_library;
  for (client_library = linklist_first_som_client_object (&client->object_list);
       client_library != NULL && client_library->library->object.handle != handle;
       client_library = linklist_next_som_client_object (client_library))
    /* */;

  som_library_object *global_library;

  /* If the object was linked to the client, then it is marked for expiry.
     If the object is not used by any client, then assume that this is a
     specific request to remove it from the system immediately.  */
  if (client_library)
    {
      /* The object in the global list.  */
      global_library = client_library->library;

      /* Remove object from client list.  */
      linklist_remove (&client->object_list, &client_library->link);

      /* Free the client copy of the library data.  */
      som_free (client_library->rw_addr);

      /* Free memory used to store object in client list.  */
      som_free (client_library);

      /* Now deal with global object.
         If the usage count is not zero, then another client is using the
         library and it should not be marked for expiry.  */
      if (--global_library->usage_count)
        return NULL;

      /* Usage count is zero, so object is no longer in use at all. We
         don't remove it from memory straight away, but mark it for
         expiry.  */
      global_library->expire_time = os_read_monotonic_time () + global.max_idle_time;
    }
  else
    {
      for (global_library = linklist_first_som_library_object (&global.object_list);
	   global_library != NULL && global_library->object.handle != handle;
	   global_library = linklist_next_som_library_object (global_library))
	/* */;

      /* In theory, there shouldn't be another client using the object (i.e.,
	 usage_count > 0). If the dynamic loader is trying to remove a bogus
	 library, then it must be for the first client that tried to use it.
	 However, we will make no assumptions. If the usage count is not 0,
	 then leave it alone.  */
      if (global_library && global_library->usage_count == 0)
	{
	  /* Remove from the global list.  */
	  linklist_remove (&global.object_list, &global_library->link);

	  /* Mark this slot in the array as re-usable.  */
	  global.object_array.object_base[global_library->object.index] = NULL;

	  som_free (global_library->object.base_addr);
	  som_free (global_library);
	}
    }

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

  som_client_object *client_library =
		linklist_first_som_client_object (&client->object_list);
  while (client_library)
    {
      som_client_object *next_library =
		linklist_next_som_client_object (client_library);

      deregister_shared_object (client, client_library->library->object.handle);
      /* Ignore any errors from above.  */

      client_library = next_library;
    }

  /* Unlink from the list of clients. */
  linklist_remove (&global.client_list, &client->link);

  somarray_fini (&client->runtime_array);

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
