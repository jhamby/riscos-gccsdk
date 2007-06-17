/* som_workspace.h
 *
 * Copyright 2007 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_WORKSPACE_H
#define SOM_WORKSPACE_H

#define	RT_WORKSPACE_START	0xD4

enum
{
  /* An array of GOT pointers used by the current client. */
  rt_workspace_GOT_PTR_ARRAY,

  /* Pointer to the current client structure. */
  rt_workspace_CLIENT_STRUCT_PTR,
};

static inline void rt_workspace_set(int pos, unsigned int value)
{
  *((unsigned int *)(0x8000 + RT_WORKSPACE_START) + pos) = value;
}

static inline unsigned int rt_workspace_get(int pos)
{
  return *((unsigned int *)(0x8000 + RT_WORKSPACE_START) + pos);
}

#endif
