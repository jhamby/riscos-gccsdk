/* som_utilswis.c
 *
 * Copyright 2007 GCCSDK Developers
 */

#include "som_utilswis.h"
#include <string.h>
#include "som.h"
#include "somanager.h"

/* Retrieve information about the library whose handle is
 * given. The data is placed in a user supplied buffer.
 *
 * entry:
 *  r0 = handle of library
 *  r1 = pointer to buffer to return library information in
 *   r1 + 0 = base address
 *   r1 + 4 = pointer to library's read/write segment
 *   r1 + 8 = pointer to client copy of read/write segment (if applicable)
 *   r1 + 12 = size of read/write segment
 *   r1 + 16 = offset of GOT from start of read/write segment
 *   r1 + 20 = offset of bss area from start of read/write segment
 *   r1 + 24 = size of bss area
 *   r1 + 28 = pointer to name (Read only)
 *   r1 + 32 = flags
 *  r2 = flags
 *   bit 0 - set to search current client object list
 *           clear to search global object list
 * exit:
 *  all registers preserved if object found and no error, or r0 = pointer to error block
 */
_kernel_oserror *som_query_object(_kernel_swi_regs *regs)
{
unsigned int handle = (unsigned int)regs->r[0];
som_objinfo *objinfo = (som_objinfo *)regs->r[1];
som_object *object;

  if (handle == 0 || objinfo == NULL)
    return somerr_bad_param;

  if ((regs->r[2] & 1) == 1)
  {
  som_client *client = FIND_CLIENT();

    object = linklist_first_som_object(&client->object_list);
  }
  else
    object = linklist_first_som_object(&global.object_list);

  while (object)
  {
    if (object->handle == handle)
      break;

    object = linklist_next_som_object(object);
  }

  if (!object)
    return somerr_object_not_found;

  objinfo->base_addr = object->base_addr;
  objinfo->public_rw_ptr = object->rw_addr;
  objinfo->private_rw_ptr = object->private_rw_ptr;
  objinfo->rw_size = object->rw_size;
  objinfo->got_offset = object->got_addr - objinfo->public_rw_ptr;
  objinfo->bss_offset = object->bss_addr - objinfo->public_rw_ptr;
  objinfo->bss_size = object->bss_size;
  objinfo->dyn_offset = object->dynamic_addr - objinfo->public_rw_ptr;
  objinfo->dyn_size = object->dynamic_size;
  objinfo->name = object->name;
  objinfo->flags = object->flags;

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
 *  r1 = internal handle of requested object for use as input in subsequent calls
 *	(NULL if there is no object to return)
 *
 * (Note: internal handle is actually base address of object)
 */
_kernel_oserror *som_iterate_objects(_kernel_swi_regs *regs)
{
_kernel_oserror *err = NULL;
som_object *result_object = NULL;

  switch (regs->r[0])
  {
  case reason_code_SOM_ITERATE_NEXT:
    {
    som_object *object = (som_object *)regs->r[1];

      if (object)
	result_object = linklist_next_som_object(object);
      else
	err = somerr_bad_param;
    }
    break;

  case reason_code_SOM_ITERATE_PREV:
    {
    som_object *object = (som_object *)regs->r[1];

      if (object)
	result_object = linklist_prev_som_object(object);
      else
	err = somerr_bad_param;
    }
    break;

  case reason_code_SOM_ITERATE_FIRST:
    {
    som_client *client = FIND_CLIENT();

      if (client)
	result_object = linklist_first_som_object(&client->object_list);
      else
	err = somerr_client_not_found;
    }
    break;

  case reason_code_SOM_ITERATE_LAST:
    {
    som_client *client = FIND_CLIENT();

      if (client)
	result_object = linklist_last_som_object(&client->object_list);
      else
	err = somerr_client_not_found;
    }
    break;

  default:
    err = somerr_bad_param;
    break;
  }

  if (err)
    regs->r[0] = regs->r[1] = 0;
  else if ((regs->r[1] = (unsigned int)result_object) != 0)
    regs->r[0] = result_object->handle;

  return err;
}

/* Given an address return the client's private GOT of the library that contains it
 * entry:
 *  r0 = address
 * exit:
 *  r0 = ptr to GOT or 0 if failed
 */
_kernel_oserror *som_got_from_addr(_kernel_swi_regs *regs)
{
som_client *client;
som_object *object;
som_PTR addr = (som_PTR)regs->r[0];

  if ((client = FIND_CLIENT()) == NULL)
  {
    regs->r[0] = 0;
    return somerr_client_not_found;
  }

  object = linklist_first_som_object(&client->object_list);
  while (object)
  {
    if (addr <= object->end_addr)
      break;

    object = linklist_next_som_object(object);
  }

_kernel_oserror *err;

  if (object)
  {
    regs->r[0] = (unsigned int)object->private_got_ptr;
    err = NULL;
  }
  else
  {
    regs->r[0] = 0;
    err = somerr_object_not_found;
  }

  return err;
}

/* Given an address return the handle of the library that contains it
 * entry:
 *  r0 = address
 * exit:
 *  r0 = handle
 */
_kernel_oserror *som_handle_from_addr(_kernel_swi_regs *regs)
{
som_client *client;
som_object *object;
som_PTR addr = (som_PTR)regs->r[0];

  if ((client = FIND_CLIENT()) == NULL)
  {
    regs->r[0] = 0;
    return somerr_client_not_found;
  }

  object = linklist_first_som_object(&client->object_list);
  while (object)
  {
    if (addr <= object->end_addr)
      break;

    object = linklist_next_som_object(object);
  }

_kernel_oserror *err;

  if (object)
  {
    regs->r[0] = object->handle;
    err = NULL;
  }
  else
  {
    regs->r[0] = 0;
    err = somerr_object_not_found;
  }

  return err;
}

/* Given ptr to library name, return handle of library
 * from global list
 * entry:
 *  r0 = ptr to name (Read Only)
 * exit:
 *  r0 = handle or 0 for failure
 */
_kernel_oserror *som_handle_from_name(_kernel_swi_regs *regs)
{
som_object *object;
const char *name = (const char *)regs->r[0];
som_handle handle = 0;

  /* Search global list, not current client. */
  object = linklist_first_som_object(&global.object_list);
  while (object)
  {
    if (strcmp(object->name, name) == 0)
    {
      handle = object->handle;
      break;
    }

    object = linklist_next_som_object(object);
  }

  regs->r[0] = (unsigned int)handle;

  return NULL;
}

/* entry:
 *  r0 = address
 * exit:
 *  r0 = offset
 */
_kernel_oserror *som_addr_to_offset(_kernel_swi_regs *regs)
{
som_client *client = FIND_CLIENT();
unsigned int offset = 0;
som_PTR addr = (som_PTR)regs->r[0];

  if (client)
  {
  som_object *object = linklist_first_som_object(&client->object_list);

    while (object)
    {
      if (addr <= object->end_addr)
      {
	offset = addr - object->base_addr;
	break;
      }

      object = linklist_next_som_object(object);
    }
  }

  regs->r[0] = offset;

  return NULL;
}
