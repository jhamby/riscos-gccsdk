/* som_array.h
 *
 * Copyright 2007-2011 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_ARRAY_H
#define SOM_ARRAY_H

#include "som_types.h"

typedef struct _som_rt_elem
{
  /* The GOT pointer must be the first member so that the PIC code will
     work.  */
  som_PTR private_GOT_ptr;
  som_PTR public_RW_ptr;
  som_PTR private_RW_ptr;
  unsigned int RW_size;
} som_rt_elem;

typedef struct _som_array
{
  int elem_size;

  int elem_count;

  union
  {
    void **base;
    som_rt_elem *rt_base;	/* An array of runtime elements.  */
    struct _som_object **object_base;	/* An array of som_object pointers.  */
  };
} som_array;

/* Allocate memory for the array using the given size as the starting
   size.  */
extern _kernel_oserror *somarray_init (som_array *array, int elem_size,
				       int elem_count);

/* Free resources used by the array.  */
extern void somarray_fini (som_array *array);

/* Extend the array and add the given object.  */
extern _kernel_oserror *somarray_add_object (som_array *array,
					     struct _som_object *obj);

/* SWI "SOM_GenerateRuntimeArray" has to be called at least twice - once after
   the dynamic loader has registered but before it is called so that it is
   able to run correctly and then again after the dynamic loader has
   finished linking but before the user program is entered in order to pick
   up any libraries that were loaded. If dl_open() is used, it must be
   called again to regenerate the array.  */
extern _kernel_oserror *som_generate_runtime_array (void);

#endif
