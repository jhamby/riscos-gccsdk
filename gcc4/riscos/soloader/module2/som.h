/* som.h
 *
 * Copyright 2007 GCCSDK Developers
 */

#ifndef SOM_H
#define SOM_H

#define LIBRARIES_IN_DA
#define USE_SYMLINKS

#include "link_list.h"
#include "somanager.h"
#include "som_os_swis.h"
#include "som_workspace.h"

#define SOM_MAX_DA_SIZE			1024 * 1024 		/* 1Mb */
#define SOM_INIT_DA_SIZE		4 * 1024 		/* 4Kb (multiple of 4Kb) */

#define SOM_MAX_LIBDA_SIZE		1024 * 1024 * 128 	/* 128MB */
#define SOM_INIT_LIBDA_SIZE		4 * 1024 		/* 4KB */

#define SOM_CALL_EVERY_CS_DELAY		(1 * 100 * 60) + 1	/* 1 minute */
#define SOM_MAX_IDLE_TIME		(30 * 100 * 60)		/* 30 minutes */
#define SOM_BAD_VALUE			0x1BADBAD1
#define SOM_HEAP_INC			(64 * 1024)		/* 64Kb (multiple of 4Kb) */
#define SOM_ALLOCATOR_SOMX		0x584D4F53		/* SOMX */
#define SOM_ALLOCATOR_SOMD		0x444D4F53		/* SOMD */
#define SOM_ALLOCATOR_SOML		0x4C4D4F53		/* SOML */

typedef unsigned int bool;
#define true 1
#define false 0

typedef unsigned int som_client_ID;
typedef unsigned int som_handle;
typedef char *som_PTR;

enum
{
  reason_code_SOM_REGISTER_LOADER,
  reason_code_SOM_REGISTER_CLIENT,
  reason_code_SOM_REGISTER_LIBRARY
};

enum
{
  flag_QUERY_GLOBAL_LIST,
  flag_QUERY_CLIENT_LIST
};
#define mask_QUERY_LIST	1

enum
{
  object_flag_type_LOADER =	1,
  object_flag_type_CLIENT =	2,
  object_flag_type_SHARED =	4
};

enum
{
  reason_code_SOM_ITERATE_FIRST,
  reason_code_SOM_ITERATE_NEXT,
  reason_code_SOM_ITERATE_PREV,
  reason_code_SOM_ITERATE_LAST
};

/* A link in the list of known clients */
typedef struct _som_client som_client;
struct _som_client
{
  link_hdr		link;

  /* Pointer to filename of client */
  char *		name;

  /* ID code for this client */
  som_client_ID		unique_ID;

  /* Ordered (by base addr) linked list of libraries used by this client.
     First object is for the client itself. */
  link_list		object_list;
};

LINKLIST_ACCESS_FUNCTIONS(som_client)

typedef struct som_object_flags
{
  unsigned int type : 4;

} som_object_flags;

/* A link in the list of libraries that are currently loaded */
typedef struct _som_object som_object;
struct _som_object
{
  link_hdr		link;

  /* Handle of object (currently base address of object, 0x8000 for app, 0 = invalid) */
  som_handle		handle;

  /* Pointer to start of library's address space */
  som_PTR		base_addr;

  /* Pointer to end of library's address space */
  som_PTR		end_addr;

  /* Pointer to library's read/write segment */
  som_PTR		rw_addr;

  /* Size of the read/write segment */
  int			rw_size;

  /* Pointer to library's GOT */
  som_PTR		got_addr;

  /* Pointer to library's bss area */
  som_PTR		bss_addr;

  /* Size of bss area */
  int			bss_size;

  /* Address of dynamic segment */
  som_PTR		dynamic_addr;

  /* Size of dynamic segment */
  int			dynamic_size;

  /* The following two fields are only used by library objects,
     they have no meaning to clients. */

  /* Number of clients that are using this library */
  int			usage_count;

  /* The time (roughly) at which the library will be removed from
     memory if it's usage count doesn't increase above 0 before
     hand */
  unsigned int		expire_time;

  char *		name;

  som_object_flags	flags;

  /* These are only used in client lists, not in the global list */
  som_PTR		private_rw_ptr;
  som_PTR		private_got_ptr;
};

LINKLIST_ACCESS_FUNCTIONS(som_object)

typedef struct _som_objinfo som_objinfo;
struct _som_objinfo
{
  som_PTR		base_addr;

  /* The library's copy of the r/w segment */
  som_PTR		public_rw_ptr;

  /* The client's copy of the r/w segment */
  som_PTR		private_rw_ptr;

  /* Size of the r/w segment */
  int			rw_size;

  /* Offset in bytes of the GOT into the r/w segment */
  int			got_offset;

  /* Offset in bytes of the bss area into the r/w segment */
  int			bss_offset;

  /* Size in bytes of the bss area */
  int			bss_size;

  /* Offset in bytes of dynamic segment into R/W segment */
  int			dyn_offset;

  /* Size in bytes of dynamic segment */
  int			dyn_size;

  /* Pointer to name of object */
  char *		name;

  som_object_flags	flags;
};

typedef struct _som_globals som_globals;
struct _som_globals
{
  /* Dynamic area for general data allocations (not library code) */
  dynamic_area_block	data_da;

  /* Dynamic area for library code (32bit OS only) */
  dynamic_area_block	library_da;

  /* List of clients known to system */
  link_list		client_list;

  /* List of libraries loaded by system */
  link_list		object_list;

  /* The last client struct ptr that was found when searching the list */
  som_client *		cached_client_ptr;

  /* The ID of the last client found when searching the list */
  som_client_ID		cached_client_ID;

  /* Number of centiseconds a library is idle in memory before being removed. */
  unsigned int		max_idle_time;

  /* Number of centiseconds the garbage collection ticker occurs. */
  unsigned int		call_every_cs_delay;

  struct
  {
    bool no_client_check	: 1;
    bool host_32bit		: 1;
    bool callback_pending	: 1;
    bool call_every_enabled	: 1;

  } flags;
};

extern som_globals global;

/* Return the client object whose ID is given. */
extern som_client *som_find_client(som_handle ID);

extern _kernel_oserror *som_prologue(_kernel_swi_regs *);
extern _kernel_oserror *som_resolver(_kernel_swi_regs *);

extern _kernel_oserror *som_start_call_every(void *pw);
extern _kernel_oserror *som_stop_call_every(void *pw);

/* Return the client object whose ID is given by using cached values for speed,
   fall back on som_find_client() if that fails. */
static inline som_client *FIND_CLIENT(void)
{
  if (rt_workspace_get(rt_workspace_CLIENT_ID) == global.cached_client_ID)
    return global.cached_client_ptr;

  return som_find_client(rt_workspace_get(rt_workspace_CLIENT_ID));
}

static inline som_PTR word_align(som_PTR addr)
{
  return (som_PTR)(((unsigned int)addr + 3) & ~3);
}

#endif
