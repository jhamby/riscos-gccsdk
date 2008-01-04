/* som_array.c
 *
 * Copyright 2007 GCCSDK Developers
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

  /* Store the object index in the library public GOT.  */
  ((unsigned int *) object->got_addr)[SOM_OBJECT_INDEX_OFFSET] = object->index;

  /* Store the location of the client runtime array in the public GOT.  */
  ((unsigned int *) object->got_addr)[SOM_RUNTIME_ARRAY_OFFSET] = rt_workspace_find();

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

  if ((err = somarray_init (&client->runtime_array, sizeof (som_rt_elem),
			    object_array->elem_count)) != NULL)
    return err;

  /* First object is always the client.  */
  rt_elem_set (&client->runtime_array.rt_base[0],
	       linklist_first_som_object (&client->object_list));

  for (int i = 1; i < object_array->elem_count; i++)
    {
      if (object_array->object_base[i] == NULL)
	{
	  /* A NULL object in the array suggests an object that was used
	     by a different client and has since expired.  */
	  rt_elem_clear (&client->runtime_array.rt_base[i]);
	}
      else
	{
	  /* Check if the client uses this object by comparing indices.  */
	  som_object *object;
	  for (object = linklist_first_som_object (&client->object_list);
	       object != NULL && object->index != i;
	       object = linklist_next_som_object (object))
	    /* */;

	  if (object)
	    rt_elem_set (&client->runtime_array.rt_base[i], object);
	  else
	    rt_elem_clear (&client->runtime_array.rt_base[i]);
	}
    }

  rt_workspace_set (rt_workspace_RUNTIME_ARRAY,
		    (unsigned int) client->runtime_array.rt_base);

  return NULL;
}
