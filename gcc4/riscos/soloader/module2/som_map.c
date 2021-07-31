/* som_map.c
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 * 
 * This code is not currently compiled or used.
 * 
 */

#include "som.h"
#include "som_map.h"
#include "som_alloc.h"
#include "som_os_swis.h"
#include <stdio.h>

#define DEBUG_PRINT 0
#define DEBUG_REPORT 0

#define DEBUG_MAPPING (DEBUG_PRINT || DEBUG_REPORT)

#if DEBUG_MAPPING
#if DEBUG_PRINT
#define DEBUG_PRINT_IT puts(buffer)
#else
#define DEBUG_PRINT_IT
#endif
#if DEBUG_REPORT
#define DEBUG_REPORT_IT report_text(buffer)
#else
#define DEBUG_REPORT_IT
#endif
#define DEBUG_OUTPUT(format, args...) \
{\
char buffer[256];\
sprintf(buffer,format,args);\
DEBUG_REPORT_IT;\
DEBUG_PRINT_IT;\
}
#else
#define DEBUG_OUTPUT(format, args...)
#endif

_kernel_oserror *
som_map_find_mappings (som_client_object *client_library)
{
  armeabi_allocator_t allocator = global.memory_page_allocator;
  _kernel_oserror *err;
  int num_pages = bytes_to_pages(client_library->library->object.data_size);

  if ((err = som_alloc (sizeof(pmp_log_page_entry) * num_pages * 2,
			(void **)&client_library->map)) != NULL)
    return err;

  client_library->unmap = client_library->map + num_pages;
  client_library->num_pages = num_pages;

  void *addr_src = client_library->data_segment;
  void *addr_dst = client_library->library->object.text_segment +
			page_align_size(client_library->library->object.text_size);


  int i, pmp_index, da_page_num;
  armeabi_memory_allocator_get_page_number(allocator, addr_src, &pmp_index);
  armeabi_memory_allocator_get_page_number(allocator, addr_dst, &da_page_num);

  DEBUG_OUTPUT("%s: src (%p); page (%d), dst (%p); page (%d), no. pages (%d)",
		__func__, addr_src, pmp_index,
		addr_dst, da_page_num,
		num_pages);
  for (i = 0/*,
         pmp_index = som_page_number(allocator, addr_src),
         da_page_num = som_page_number(allocator, addr_dst)*/;
       i < num_pages;
       i++, pmp_index++, da_page_num++)
    {
      client_library->map[i].da_page_number = da_page_num;
      client_library->map[i].pmp_page_index = pmp_index;
      client_library->map[i].flags = PMP_MEM_ACCESS_RWE_RWE;

      /* The unmap array reverses the above page mappings so that the pages
         are returned to their original positions.  */
      client_library->unmap[i].da_page_number = pmp_index;
      client_library->unmap[i].pmp_page_index = pmp_index;
      client_library->unmap[i].flags = PMP_MEM_ACCESS_RWE_RWE;
    }

  return NULL;
}
extern _kernel_oserror *dynamic_pmp_map_unmap_ASM(const int __da,
		       pmp_log_page_entry *__page_list,
		       int __num_entries);
_kernel_oserror *
som_map_object (som_client_object *client)
{
  _kernel_oserror *err = NULL;

  DEBUG_OUTPUT("%s:%d - client->mapped (%d)",__func__,__LINE__,client->mapped);

  if (!client->mapped)
    {
      int da_number;
      som_PTR base, end;
      err = armeabi_memory_info(global.memory_page_allocator, &da_number, &base, &end);
/*      DEBUG_OUTPUT("%s: (%p) %d -> (%p) %d (%d pages)",
		   __func__,
		   global.paged_memory.da.base_addr + (client->map[0].pmp_page_index << 12),
		   client->map[0].pmp_page_index,
		   global.paged_memory.da.base_addr + (client->map[0].da_page_number << 12),
		   client->map[0].da_page_number,
		   client->num_pages);*/

      err = dynamic_pmp_map_unmap (da_number,
				   client->map,
				   client->num_pages);
      if (!err)
	client->mapped = true;
    }

  return err;
}

_kernel_oserror *
som_unmap_object (som_client_object *client)
{
  _kernel_oserror *err = NULL;

  DEBUG_OUTPUT("%s:%d - client->mapped (%d)",__func__,__LINE__,client->mapped);

  if (client->mapped)
    {
      int da_number;
      som_PTR base, end;
      err = armeabi_memory_info(global.memory_page_allocator, &da_number, &base, &end);
/*      DEBUG_OUTPUT("%s: (%p) %d -> (%p) %d (%d pages)",
		   __func__,
		   global.paged_memory.da.base_addr + (client->unmap[0].da_page_number << 12),
		   client->unmap[0].da_page_number,
		   global.paged_memory.da.base_addr + (client->unmap[0].pmp_page_index << 12),
		   client->unmap[0].pmp_page_index,
		   client->num_pages);*/

      err = dynamic_pmp_map_unmap (da_number,
				   client->unmap,
				   client->num_pages);
      if (!err)
	client->mapped = false;
#if DEBUG_MAPPING
      else
	printf("%s: RISC OS Error: %s\n", __func__, err->errmess);
#endif
    }

  return err;
}

_kernel_oserror *
som_map_object_from_address (som_PTR addr)
{
  som_client_object *client_library;
  som_client *client;

  if ((client = FIND_CLIENT ()) == NULL)
    return somerr_client_not_found;

  for (client_library = linklist_first_som_client_object (&client->object_list);
       client_library != NULL && addr > client_library->library->object.end_addr;
       client_library = linklist_next_som_client_object (client_library))
    /* */;

  if (client_library)
    return som_map_object (client_library);

  return somerr_object_not_found;
}
