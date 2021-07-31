/* som_final.c
 *
 * Copyright 2006-2019 GCCSDK Developers
 * Written by Lee Noar
 */

#include <stdlib.h>
#include <stdio.h>
#include <kernel.h>
#include <swis.h>
#include "somanager.h"
#include "som.h"
#include "som_os_swis.h"
#include "som_alloc.h"

_kernel_oserror *
module_finalisation (int fatal, int podule_base, void *pw)
{
  if (!global.flags.no_client_check && global.client_list.count > 0)
    return somerr_in_use;

  som_stop_call_every (pw);

  somarray_fini (&global.object_array);

  if (global.flags.host_32bit)
    {
      som_library_object *global_library =
		linklist_first_som_library_object (&global.object_list);

      while (global_library)
	{
	  som_library_object *next = linklist_next_som_library_object (global_library);

	  som_free (global_library->object.text_segment);
	  som_free ((som_PTR)global_library);

	  global_library = next;
	}

      if (global.memory_page_allocator)
	armeabi_memory_destroy_allocator(global.memory_page_allocator);
    }
  else
    {
      som_library_object *global_library =
		linklist_first_som_library_object (&global.object_list);

      while (global_library)
	{
	  if (global_library->object.flags.type != object_flag_type_CLIENT
	      && global_library->object.text_segment)
	    som_free (global_library->object.text_segment);

	  global_library = linklist_next_som_library_object (global_library);
	}
    }

  dynamic_area_remove (global.data_da.number);

  return NULL;
}
