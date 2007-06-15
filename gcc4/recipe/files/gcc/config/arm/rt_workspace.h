@ rt_workspace.h for RISC OS
@ Copyright (c) 2006, 2007 GCCSDK Developers

@===================================================================
@ A structure that defines the layout of runtime data at the
@ bottom of a client wimpslot. These offsets are from the base of
@ the wimpslot, ie 0x8000, and must be kept in sync with those used
@ in the shared library support package.
@===================================================================
				.struct	0
				.skip	0xD4	@ skip ELF header
RT_WORKSPACE_LIBRARY_INDEX_TABLE:.skip	4

/* Pointer to the current client structure. */
RT_WORKSPACE_CLIENT_STRUCT_PTR:	.skip	4

RT_WORKSPACE_CLIENT_ID:		.skip	4

@ This is the client's object list. It remains constant for the life
@ of the client, so keeping a pointer to its head is not a problem.
RT_WORKSPACE_OBJECT_LIST:	.skip	4

@ These 2 values are used and maintained by _rt_load_pic
RT_WORKSPACE_CACHED_R7:		.skip	4
RT_WORKSPACE_CACHED_GOT:	.skip	4

@ This is a pointer to the client's private copy of the dynamic loader's
@ GOT. It is returned by _rt_load_pic as a last resort if it fails to find
@ any other library. In this case, it's assumed that program initialisation
@ is still occurring.
RT_WORKSPACE_LD_GOT:		.skip	4

@ These 2 values are used and maintained by SOM_Prologue. They were part
@ of the module, but clashes occurred between clients that were running at
@ the same time. Putting them here ensures that they stay private to each
@ client.
RT_WORKSPACE_CACHED_PLT:	.skip	4
RT_WORKSPACE_CACHED_PLTGOT:	.skip	4

@ Reserve 1 word of space, there are 10 words altogether.
RT_WORKSPACE_RESERVED:		.skip	4
