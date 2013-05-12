/* som_array.c
 *
 * Copyright 2007-2013 GCCSDK Developers
 * Written by Lee Noar
 */

#include <string.h>
#include <sys/elf.h>
#include "som.h"
#include "som_array.h"
#include "som_alloc.h"
#include "som_workspace.h"

_kernel_oserror *
somarray_init (som_array *array, int elem_size, int elem_count)
{
  _kernel_oserror *err;

  if (elem_count == 0)
    array->base = NULL;
  else
    {
      if ((err = som_alloc (elem_size * elem_count,
			    (void **) (void *) &array->base)) != NULL)
	return err;

      memset (array->base, 0, elem_size * elem_count);
    }

  array->elem_size = elem_size;
  array->elem_count = elem_count;

  return NULL;
}

void
somarray_fini (som_array *array)
{
  if (array->base)
    {
      som_free (array->base);
      array->base = NULL;
    }
}

_kernel_oserror *
somarray_add_object (som_array *array,
		     som_object *object)
{
  /* Check for unused elements that can be recycled, index 0 is always
     the client so we start searching from index 1.  */
  int index;
  for (index = 1;
       index < array->elem_count && array->object_base[index] != NULL;
       ++index)
    /* */;

  if (index >= array->elem_count)
    {
      /* There are no elements that can be recycled, so extend the array.  */
      _kernel_oserror *err;

      if ((err = som_extend ((void **) (void *) &array->object_base,
			     sizeof (som_object *))) != NULL)
	return err;

      index = array->elem_count++;
    }

  object->index = index;

  array->object_base[index] = object;

  /* Store the object index in the library runtime workspace.  */
  rt_workspace_set_for_object (object->base_addr, rt_workspace_OBJECT_INDEX, index);

  return NULL;
}

static void
rt_elem_set_client (const som_client *client)
{
  som_rt_elem *rt_elem = &client->runtime_array.rt_base[0];

  rt_elem->private_GOT_ptr = client->object.got_addr;
  rt_elem->public_RW_ptr = client->object.rw_addr;
  rt_elem->private_RW_ptr = client->object.rw_addr;
  rt_elem->RW_size = client->object.rw_size;
}

static void
rt_elem_set (som_rt_elem *rt_elem, const som_client_object *c_obj)
{
  rt_elem->private_GOT_ptr = c_obj->got_addr;
  rt_elem->public_RW_ptr = c_obj->library->object.rw_addr;
  rt_elem->private_RW_ptr = c_obj->rw_addr;
  rt_elem->RW_size = c_obj->library->object.rw_size;
}

static void
rt_elem_clear (som_rt_elem *rt_elem)
{
  memset (rt_elem, 0, sizeof (som_rt_elem));
}

_kernel_oserror *
som_generate_runtime_array (void)
{
  som_array *object_array = &global.object_array;
  _kernel_oserror *err;
  som_client *client;

  if ((client = FIND_CLIENT ()) == NULL)
    return somerr_unknown_client;

  somarray_fini (&client->gott_base);
  somarray_fini (&client->runtime_array);
  
  if ((err = somarray_init (&client->gott_base, sizeof (void *),
			    object_array->elem_count)) != NULL
   || (err = somarray_init (&client->runtime_array, sizeof (som_rt_elem),
			    object_array->elem_count)) != NULL)
    goto cleanup;

  som_client_object *c_obj = linklist_first_som_client_object (&client->object_list);

  /* First object is always the client.  */
  rt_elem_set_client (client);
  client->gott_base.base[0] = client->object.got_addr;

  /* The arrays are zero'd when allocated, so we don't need to clear unused
     elements. We only need to worry about setting the elements that are used.  */
  for (int i = 1; i < object_array->elem_count; i++)
    {
      if (object_array->object_base[i] != NULL)
	{
	  /* Check if the client uses this object. If the index of the current
	     object matches an index in the client list, then the object is used
	     by the client.  */
	  for (c_obj = linklist_first_som_client_object (&client->object_list);
	       c_obj != NULL && c_obj->library->object.index != i;
	       c_obj = linklist_next_som_client_object (c_obj))
	    /* */;

	  if (c_obj)
	    {
	      rt_elem_set (&client->runtime_array.rt_base[i], c_obj);
	      client->gott_base.base[i] = c_obj->got_addr;
	    }
	}
    }

  rt_workspace_set (rt_workspace_GOTT_BASE,
		    (unsigned int) client->gott_base.base);
  rt_workspace_set (rt_workspace_RUNTIME_ARRAY,
		    (unsigned int) client->runtime_array.rt_base);

  return NULL;
cleanup:
  somarray_fini (&client->gott_base);
  somarray_fini (&client->runtime_array);

  return err;
}
