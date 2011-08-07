/* som_array.c
 *
 * Copyright 2007-2011 GCCSDK Developers
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
  som_free (array->base);
}

_kernel_oserror *
somarray_add_object (som_array *array, som_object *object)
{
  /* Check for unused elements that can be recycled, index 0 is always
     the client so we start searching from index 1.  */
  int index;
  for (index = 1;
       index < array->elem_count && array->object_base[index] != NULL;
       ++index)
    /* */;

  if (index < array->elem_count)
    object->index = index;
  else
    {
      _kernel_oserror *err;

      /* There are no elements that can be recycled, so extend the array.  */
      if ((err = som_extend ((void **) (void *) &array->object_base,
			     sizeof (som_object *))) != NULL)
	return err;

      object->index = array->elem_count++;
    }

  array->object_base[object->index] = object;

  /* Store the object index in the library runtime workspace.  */
  rt_workspace_set_for_object (object->base_addr, rt_workspace_OBJECT_INDEX, object->index);

  return NULL;
}

static void
rt_elem_set (som_rt_elem *rt_elem, const som_object *object)
{
  rt_elem->private_GOT_ptr = object->private_got_ptr;
  rt_elem->public_RW_ptr = object->rw_addr;
  rt_elem->private_RW_ptr = object->private_rw_ptr;
  rt_elem->RW_size = object->rw_size;
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

  if (client->runtime_array.rt_base)
    som_free (client->runtime_array.rt_base);
  
  if ((err = somarray_init (&client->gott_base, sizeof (void *),
			    object_array->elem_count)) != NULL
   || (err = somarray_init (&client->runtime_array, sizeof (som_rt_elem),
			    object_array->elem_count)) != NULL)
    return err;

  som_object *object = linklist_first_som_object (&client->object_list);

  /* First object is always the client.  */
  rt_elem_set (&client->runtime_array.rt_base[0],
	       object);
  client->gott_base.base[0] = object->private_got_ptr;

  /* The arrays are zero'd when allocated, so we don't need to clear unused
     elements. We only need to worry about setting the elements that are used.  */
  for (int i = 1; i < object_array->elem_count; i++)
    {
      if (object_array->object_base[i] != NULL)
	{
	  /* Check if the client uses this object. If the index of the current
	     object matches an index in the client list, then the object is used
	     by the client.  */
	  for (object = linklist_first_som_object (&client->object_list);
	       object != NULL && object->index != i;
	       object = linklist_next_som_object (object))
	    /* */;

	  if (object)
	    {
	      rt_elem_set (&client->runtime_array.rt_base[i], object);
	      client->gott_base.base[i] = object->private_got_ptr;
	    }
	}
    }

  rt_workspace_set (rt_workspace_GOTT_BASE,
		    (unsigned int) client->gott_base.base);
  rt_workspace_set (rt_workspace_RUNTIME_ARRAY,
		    (unsigned int) client->runtime_array.rt_base);

  return NULL;
}
