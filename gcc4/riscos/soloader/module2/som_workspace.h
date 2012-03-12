/* som_workspace.h
 *
 * Copyright 2007-2011 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_WORKSPACE_H
#define SOM_WORKSPACE_H

#include <sys/elf.h>

#define RT_WORKSPACE_SIZE	20

enum
{
  /* All objects that are in use have an index used to access the runtime
     and gott_base arrays.  */
  rt_workspace_OBJECT_INDEX,

  /* An array of Global Offset Table pointers used at runtime by the PIC
     instruction sequence.  */
  rt_workspace_GOTT_BASE,

  /* An array used by the dynamic linker to aid in relocating libraries.  */
  rt_workspace_RUNTIME_ARRAY,

  /* Pointer to the current client structure.  */
  rt_workspace_CLIENT_STRUCT_PTR
};

#define RT_WORKSPACE_GOTT_BASE	(0x8000 + sizeof (Elf32_Ehdr) + (rt_workspace_GOTT_BASE * 4))
#define RT_WORKSPACE_RUNTIME_ARRAY (0x8000 + sizeof (Elf32_Ehdr) + (rt_workspace_RUNTIME_ARRAY * 4))

/* The ELF header is located at the load address of the object and is immediately
   followed by the runtime workspace.  */
static inline unsigned int *
rt_workspace_find_for_object (const void *load_addr)
{
  return (unsigned int *)(load_addr + sizeof (Elf32_Ehdr));
}

static inline unsigned int *
rt_workspace_find (void)
{
  return rt_workspace_find_for_object ((void *)0x8000);
}

static inline void
rt_workspace_set_for_object (const void *load_addr,
			     int pos,
			     unsigned int value)
{
unsigned int *workspace = rt_workspace_find_for_object (load_addr);

  *(workspace + pos) = value;
}

static inline void
rt_workspace_set (int pos,
		  unsigned int value)
{
unsigned int *workspace = rt_workspace_find ();

  *(workspace + pos) = value;
}

static inline unsigned int
rt_workspace_get (int pos)
{
unsigned int *workspace = rt_workspace_find ();

  return *(workspace + pos);
}

#endif
