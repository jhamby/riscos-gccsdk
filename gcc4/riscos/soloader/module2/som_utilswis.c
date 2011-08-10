/* som_utilswis.c
 *
 * Copyright 2007, 2008 GCCSDK Developers
 * Written by Lee Noar
 */

#include <string.h>
#include "som_utilswis.h"
#include "som.h"
#include "somanager.h"

/* Retrieve information about the library whose handle is
 * given. The data is placed in a user supplied buffer, which
 * can be NULL, in which case don't return an error unless the
 * object couldn't be found. This allows the existence of an
 * object in the system to be tested without having to allocate
 * a buffer.
 */
_kernel_oserror *
som_query_object (som_handle handle,
		  som_objinfo *objinfo,
		  unsigned int flags)
{
  som_library_object *global_library = NULL;

  if (handle == 0)
    return somerr_bad_param;

  if ((flags & mask_QUERY_LIST) == flag_QUERY_CLIENT_LIST)
    {
      som_client *client;

      if ((client = FIND_CLIENT ()) == NULL)
        return somerr_client_not_found;

      som_client_object *client_library;
      for (client_library = linklist_first_som_client_object (&client->object_list);
	   client_library && client_library->library->object.handle != handle;
	   client_library = linklist_next_som_client_object (client_library))
	/* */;

      if (client_library)
	global_library = client_library->library;

      if (objinfo)
	objinfo->private_rw_ptr = client_library->rw_addr;
    }
  else
    {
      for (global_library = linklist_first_som_library_object (&global.object_list);
	   global_library && global_library->object.handle != handle;
	   global_library = linklist_next_som_library_object (global_library))
	/* */;

      if (objinfo)
	objinfo->private_rw_ptr = NULL;
    }

  if (!global_library)
    return somerr_object_not_found;

  if (!objinfo)
    return NULL;

  objinfo->base_addr = global_library->object.base_addr;
  objinfo->public_rw_ptr = global_library->object.rw_addr;
  objinfo->rw_size = global_library->object.rw_size;
  objinfo->got_offset = global_library->object.got_addr - global_library->object.rw_addr;
  objinfo->bss_offset = global_library->object.bss_addr - global_library->object.rw_addr;
  objinfo->bss_size = global_library->object.bss_size;
  objinfo->dyn_offset = global_library->object.dynamic_addr - global_library->object.rw_addr;
  objinfo->dyn_size = global_library->object.dynamic_size;
  objinfo->name = global_library->object.name;
  objinfo->flags = global_library->object.flags;

  return NULL;
}

/* entry:
 *  r0 = Reason code:
 *	0 = return first object in client list
 *	1 = return next object in list after that given in r1
 *	2 = return previous object in list before that given in r1
 *	3 = return last object in client list
 *  r1 = for r0 = 1 or r0 = 2 - internal handle as returned in r1
 * exit:
 *  r0 = handle of requested object (NULL if there is no object to return)
 *  r1 = internal handle of requested object for use as input in subsequent
 *	 calls
 *	 (NULL if there is no object to return)
 *
 * (Note: internal handle is actually base address of object)
 */
_kernel_oserror *
som_iterate_objects (_kernel_swi_regs *regs)
{
  _kernel_oserror *err = NULL;
  som_client_object *result_object = NULL;

  switch (regs->r[0])
    {
    case reason_code_SOM_ITERATE_NEXT:
      {
	som_client_object *object = (som_client_object *) regs->r[1];

	if (object)
	  result_object = linklist_next_som_client_object (object);
	else
	  err = somerr_bad_param;
      }
      break;

    case reason_code_SOM_ITERATE_PREV:
      {
	som_client_object *object = (som_client_object *) regs->r[1];

	if (object)
	  result_object = linklist_prev_som_client_object (object);
	else
	  err = somerr_bad_param;
      }
      break;

    case reason_code_SOM_ITERATE_FIRST:
      {
	som_client *client = FIND_CLIENT ();

	if (client)
	  result_object = linklist_first_som_client_object (&client->object_list);
	else
	  err = somerr_client_not_found;
      }
      break;

    case reason_code_SOM_ITERATE_LAST:
      {
	som_client *client = FIND_CLIENT ();

	if (client)
	  result_object = linklist_last_som_client_object (&client->object_list);
	else
	  err = somerr_client_not_found;
      }
      break;

    default:
      err = somerr_bad_param;
      break;
    }

  if (err || result_object == NULL)
    regs->r[0] = regs->r[1] = 0;
  else
    {
      regs->r[1] = (unsigned int) result_object;
      regs->r[0] = result_object->library->object.handle;
    }

  return err;
}

/* Given an address return the client's private GOT of the library that
 * contains it.
 * entry:
 *  r0 = address
 * exit:
 *  r0 = ptr to GOT or 0 if failed
 */
_kernel_oserror *
som_got_from_addr (_kernel_swi_regs *regs)
{
  som_client *client;
  som_client_object *client_library;
  som_PTR addr = (som_PTR) regs->r[0];

  if ((client = FIND_CLIENT ()) == NULL)
    {
      regs->r[0] = 0;
      return somerr_client_not_found;
    }

  for (client_library = linklist_first_som_client_object (&client->object_list);
       client_library != NULL && addr > client_library->library->object.end_addr;
       client_library = linklist_next_som_client_object (client_library))
    /* */;

  if (client_library == NULL)
    {
      regs->r[0] = 0;
      return somerr_object_not_found;
    }

  regs->r[0] = (unsigned int) client_library->got_addr;

  return NULL;
}

/* Given an address return the handle of the library that contains it
 * entry:
 *  r0 = address
 * exit:
 *  r0 = handle
 */
_kernel_oserror *
som_handle_from_addr (_kernel_swi_regs *regs)
{
  som_client *client;
  som_client_object *client_library;
  som_PTR addr = (som_PTR) regs->r[0];

  if ((client = FIND_CLIENT ()) == NULL)
    {
      regs->r[0] = 0;
      return somerr_client_not_found;
    }

  for (client_library = linklist_first_som_client_object (&client->object_list);
       client_library != NULL && addr > client_library->library->object.end_addr;
       client_library = linklist_next_som_client_object (client_library))
    /* */;

  if (client_library == NULL)
    {
      regs->r[0] = 0;
      return somerr_object_not_found;
    }

  regs->r[0] = client_library->library->object.handle;

  return NULL;
}

/* Given ptr to library name, return handle of library
 * from global list
 * entry:
 *  r0 = ptr to name (Read Only)
 * exit:
 *  r0 = handle or 0 for failure
 */
som_handle
som_handle_from_name (const char *name)
{
  som_library_object *global_library;

  /* Search global list, not current client.  */
  for (global_library = linklist_first_som_library_object (&global.object_list);
       global_library != NULL && strcmp (global_library->object.name, name);
       global_library = linklist_next_som_library_object (global_library))
    /* */;

  return (global_library == NULL) ? 0 : global_library->object.handle;
}

/* SWI "SOM_Reloc"
 *
 * Given an address, determine if it exists within a library's R/W data
 * segment. If this proves to be the case, then convert to the client's
 * private copy and return as the result, otherwise, return what was given.
 *
 * entry:
 *  r0 = address to relocate
 * exit:
 *  r0 = address relocated if possible, otherwise preserved.
 */
void
som_reloc (_kernel_swi_regs *regs)
{
  som_client *client = FIND_CLIENT ();

  if (client)
    {
      som_PTR addr = (som_PTR) regs->r[0];
      som_client_object *client_library;

      for (client_library = linklist_first_som_client_object (&client->object_list);
	   client_library != NULL
	     && (addr < client_library->library->object.rw_addr
	     || addr >= client_library->library->object.rw_addr + client_library->library->object.rw_size);
	   client_library = linklist_next_som_client_object (client_library))
        /* */;

      if (client_library != NULL)
	regs->r[0] = (int) (client_library->rw_addr
			 + (addr - client_library->library->object.rw_addr));
    }
}
