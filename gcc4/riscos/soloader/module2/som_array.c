/* som_array.c
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#include "som.h"
#include "som_array.h"
#include "som_alloc.h"
#include "som_workspace.h"
#include <sys/elf.h>
#include <string.h>

_kernel_oserror *somarray_init(som_array *array, int size)
{
_kernel_oserror *err;

  if (size == 0)
    array->base = NULL;
  else
  {
    if ((err = som_alloc(size * sizeof(void *), (void **)(void *)&array->base)) != NULL)
      return err;

    memset(array->base, 0, size * sizeof(void *));
  }

  array->size = size;

  return NULL;
}

void somarray_fini(som_array *array)
{
  som_free(array->base);
}

_kernel_oserror *somarray_add_object(som_array *array, som_object *object)
{
_kernel_oserror *err;

  if ((err = som_extend((void **)(void *)&array->base, sizeof(som_object *))) != NULL)
    return err;

  object->index = array->size;

  array->object_base[array->size++] = object;

  /* Store the object index in the library public GOT. */
  *((unsigned int *)object->got_addr + SOM_OBJECT_INDEX_OFFSET) = object->index;

  /* Store the location of the client GOT ptr array in the public GOT. */
  *((unsigned int *)object->got_addr + SOM_GOT_PTR_ARRAY_OFFSET) = 0x80D4;

  return NULL;
}

_kernel_oserror *som_generate_got_array(void)
{
som_array *object_array = &global.object_array;
_kernel_oserror *err;
som_client *client = FIND_CLIENT();

  if (client->got_ptr_array.got_base)
    som_free(client->got_ptr_array.got_base);

  if ((err = somarray_init(&client->got_ptr_array, object_array->size)) != NULL)
    return err;

int i;

  /* First object is always the client. */
  client->got_ptr_array.got_base[0] = linklist_first_som_object(&client->object_list)->got_addr;

  for (i = 1; i < object_array->size; i++)
  {
    if (object_array->object_base[i] == NULL)
    {
      /* A NULL object in the array suggests an object that was used by a
       * different client and has since expired.
       */
      client->got_ptr_array.got_base[i] = NULL;
    }
    else
    {
      /* Check if the client uses this object by comparing indices. */
    som_object *object = linklist_first_som_object(&client->object_list);

      while (object)
      {
	if (object->index == i)
	  break;

	object = linklist_next_som_object(object);
      }

      if (object)
	client->got_ptr_array.got_base[i] = object->private_got_ptr;
      else
	client->got_ptr_array.got_base[i] = NULL;
    }
  }

  rt_workspace_set(rt_workspace_GOT_PTR_ARRAY, (unsigned int)client->got_ptr_array.got_base);

  return NULL;
}
