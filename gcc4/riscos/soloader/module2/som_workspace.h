/* som_workspace.h
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_WORKSPACE_H
#define SOM_WORKSPACE_H

#include <sys/elf.h>

#define RT_WORKSPACE_SIZE	20

enum
{
  /* An array used at runtime by the current client.  */
  rt_workspace_RUNTIME_ARRAY,

  /* Pointer to the current client structure.  */
  rt_workspace_CLIENT_STRUCT_PTR,
};

/* The ELF header is at address 0x8000 and is immediately followed by the
   runtime workspace. The size of the header (which includes the workspace),
   is stored within the header itself. We can use this info to determine the
   address of the workspace. */
static inline unsigned int *
rt_workspace_find(void)
{
Elf32_Ehdr *elf_header = (Elf32_Ehdr *)0x8000;

  return (unsigned int *)((unsigned int)elf_header +
		elf_header->e_ehsize - RT_WORKSPACE_SIZE);
}

static inline void
rt_workspace_set (int pos, unsigned int value)
{
unsigned int *workspace = rt_workspace_find();

  *(workspace + pos) = value;
}

static inline unsigned int
rt_workspace_get (int pos)
{
unsigned int *workspace = rt_workspace_find();

  return *(workspace + pos);
}

#endif
