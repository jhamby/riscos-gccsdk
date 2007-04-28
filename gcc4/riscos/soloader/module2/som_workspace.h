/* som_workspace.h
 *
 * Copyright 2007 GCCSDK Developers
 */

#ifndef SOM_WORKSPACE_H
#define SOM_WORKSPACE_H

#define	RT_WORKSPACE_START	0xD4

enum
{
  rt_workspace_CLIENT_ID,

  /* This is the client's object list. It remains constant for the life
     of the client, so keeping a pointer to its head is not a problem. */
  rt_workspace_OBJECT_LIST,

  /* These 2 values are used and maintained by _rt_load_pic */
  rt_workspace_CACHED_R7,
  rt_workspace_CACHED_GOT,

  /* This is a pointer to the client's private copy of the dynamic loader's
     GOT. It is returned by _rt_load_pic as a last resort if it fails to find
     any other library. In this case, it's assumed that program initialisation
     is still occurring. */
  rt_workspace_LD_GOT,

  /* These 2 values are used and maintained by SOM_Prologue. They were part
     of the module, but clashes occurred between clients that were running at
     the same time. Putting them here ensures that they stay private to each
     client. */
  rt_workspace_CACHED_PLT,
  rt_workspace_CACHED_PLTGOT,

  rt_workspace_RESERVED1,
  rt_workspace_RESERVED2,
  rt_workspace_RESERVED3
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
