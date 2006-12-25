@ som_structs.h for RISC OS
@ Copyright (c) 2006 GCCSDK Developers

@ Define structures for client and the libraries it uses.
@ These structures must be kept in sync with those used in the
@ shared library support package.

@===================================================================
@ LIST_* structure
@ A list structure which encaspulates all list data
@===================================================================
			.struct	0
LIST_FIRST_LINK:	.skip	4
LIST_LAST_LINK:		.skip	4
LIST_LINK_COUNT:	.skip	4
LIST_STRUCT_SIZE:

			.struct 0
LINK_PREV:	 	.skip	4
LINK_NEXT:		.skip	4
LINK_STRUCT_SIZE:

@===================================================================
@ CLIENT_* structure
@ A link in the list of known clients
@===================================================================
				.struct	0
CLIENT_LINK_HDR:		.skip	LINK_STRUCT_SIZE

@ Pointer to filename of client
CLIENT_NAME:			.skip	4

@ ID code for this clent
CLIENT_UNIQUE_ID:		.skip	4

@ Ordered (by base addr) linked list of libraries (OBJECT_* struct)
@ used by this client. First object is for the client itself.
CLIENT_OBJ_LIST:		.skip	LIST_STRUCT_SIZE

CLIENT_STRUCT_SIZE:

@===================================================================
@ OBJECT_* structure
@ A link in the list of libraries that are currently loaded
@===================================================================
			.struct	0
OBJECT_LINK_HDR:	.skip	LINK_STRUCT_SIZE

@ Handle of object (currently base address of object, 0x8000 for app, 0 = invalid)
OBJECT_HANDLE:		.skip	4

@ Pointer to start of library's address space
OBJECT_BASE_ADDR:	.skip	4

@ Pointer to end of library's address space
OBJECT_END_ADDR:	.skip	4

@ Pointer to library's read/write segment
OBJECT_RW_PTR:		.skip	4

@ Size of the read/write segment
OBJECT_RW_SIZE:		.skip	4

@ Pointer to library's GOT
OBJECT_GOT_ADDR:	.skip	4

@ Pointer to library's bss area
OBJECT_BSS_ADDR:	.skip	4

@ Size of bss area
OBJECT_BSS_SIZE:	.skip	4

@ Address of dynamic segment
OBJECT_DYNAMIC_ADDR:	.skip	4

@ Size of dynamic segment
OBJECT_DYNAMIC_SIZE:	.skip	4

@ The following two fields are only used by library objects,
@ they have no meaning to clients.

@ Number of clients that are using this library
OBJECT_USAGE_COUNT:	.skip	4

@ The time (roughly) at which the library will be removed from
@ memory if it's usage count doesn't increase above 0 before
@ hand
OBJECT_EXPIRE_TIME:	.skip	4

OBJECT_NAME:		.skip	4

OBJECT_FLAGS:		.skip	4

@ These are only used in client lists, not in the global list
OBJECT_PRIVATE_RW_PTR:	.skip	4
OBJECT_PRIVATE_GOT_PTR:	.skip	4

OBJECT_STRUCT_SIZE:

@===================================================================
@ OBJINFO_* structure
@ A structure used to pass data about a library for registration
@===================================================================
			.struct	0
OBJINFO_BASE_ADDR:	.skip	4
OBJINFO_PUBLIC_RW_PTR:	.skip	4  @ The library's copy of the r/w segment
OBJINFO_PRIVATE_RW_PTR:	.skip	4  @ The client's copy of the r/w segment
OBJINFO_RW_SIZE:	.skip	4  @ Size of the r/w segment
OBJINFO_GOT_OFFSET:	.skip	4  @ Offset in bytes of the GOT into the r/w segment
OBJINFO_BSS_OFFSET:	.skip	4  @ Offset in bytes of the bss area into the r/w segment
OBJINFO_BSS_SIZE:	.skip	4  @ Size in bytes of the bss area
OBJINFO_DYN_OFFSET:	.skip	4  @ Offset in bytes of dynamic segemtn into R/W segment
OBJINFO_DYN_SIZE:	.skip	4  @ Size in bytes of dynamic segment
OBJINFO_NAME:		.skip	4  @ Pointer to name of object
OBJINFO_FLAGS:		.skip	4
OBJINFO_STRUCT_SIZE:
