/* som_map.h
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_MAP_H
#define SOM_MAP_H

#include "som.h"
#include "som_os_swis.h"
#include "som_types.h"

/* Find the page mappings as required by OS_DynamicArea,22 to move pages
   to the library data segment ready for use and back again to the client's private segment.  */
extern _kernel_oserror *
som_map_find_mappings (som_client_object *);

extern _kernel_oserror *
som_map_object (som_client_object *object);

extern _kernel_oserror *
som_unmap_object (som_client_object *object);

extern _kernel_oserror *
som_map_object_from_address (som_PTR addr);

#endif
