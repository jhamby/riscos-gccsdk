/* som.h
 *
 * Copyright 2007-2019 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_H
#define SOM_H

#define LIBRARIES_IN_DA
#define USE_SYMLINKS

/* Mapped libraries are not used. Although mapped libraries worked, I realised they
 * didn't scale well; as a WIMP filter is required to unmap the libraries of the
 * outgoing task to wipe the slate clean for the incoming task, the more libraries
 * used, the greater the amount of work the filter has to do and the bigger the
 * burden on the WIMP.
 * However, as I spent so much time writing the code, I'm reluctant to delete it,
 * and things may change in future that make mapped libraries more viable, so I've
 * left the code in.
 */
#define USE_MAPPED_LIBRARIES 0

#include "link_list.h"
#include "somanager.h"
#include "som_os_swis.h"
#include "som_workspace.h"
#include "som_array.h"
#include "som_types.h"
#include "som_alloc.h"

#define SOM_MAX_DA_SIZE			1024 * 1024	/* 1Mb */
#define SOM_INIT_DA_SIZE		4 * 1024	/* 4Kb (multiple of 4Kb) */

#define SOM_MAX_LIB_DA_SIZE		1024 * 1024 * 256	/* 256MB */
#define SOM_INIT_LIB_DA_SIZE		4 * 1024	/* 4KB */

#define SOM_CALL_EVERY_CS_DELAY		(1 * 100 * 60) + 1	/* 1 minute */
#define SOM_MAX_IDLE_TIME		(30 * 100 * 60)	/* 30 minutes */
#define SOM_BAD_VALUE			0x1BADBAD1
#define SOM_HEAP_INC			(64 * 1024)	/* 64Kb (multiple of 4Kb) */
#define SOM_ALLOCATOR_SOMX		0x584D4F53	/* SOMX */
#define SOM_ALLOCATOR_SOMD		0x444D4F53	/* SOMD */
#define SOM_ALLOCATOR_SOML		0x4C4D4F53	/* SOML */

/* GCC 4.1.* specific; The offset, in words, from the start of the GOT
   where the object index will be stored.  */
#define SOM_OBJECT_INDEX_OFFSET		1	/* in words */
#define SOM_RUNTIME_ARRAY_OFFSET	2

enum
{
  reason_code_SOM_REGISTER_LOADER,
  reason_code_SOM_REGISTER_CLIENT,
  reason_code_SOM_REGISTER_LIBRARY
};

enum
{
  reason_code_SOM_ELFFILE_OPEN,
  reason_code_SOM_ELFFILE_LOAD,
  reason_code_SOM_ELFFILE_CLOSE,
  reason_code_SOM_ELFFILE_ALLOC,
  reason_code_SOM_ELFFILE_GET_ABI,
  reason_code_SOM_ELFFILE_IS_ARM_AAPCS,
  reason_code_SOM_ELFFILE_GET_PUBLIC_INFO
};

enum
{
  flag_QUERY_GLOBAL_LIST,
  flag_QUERY_CLIENT_LIST
};
#define mask_QUERY_LIST	1

enum
{
  object_flag_type_LOADER = 1,
  object_flag_type_CLIENT = 2,
  object_flag_type_SHARED = 4
};

enum
{
  reason_code_SOM_ITERATE_FIRST,
  reason_code_SOM_ITERATE_NEXT,
  reason_code_SOM_ITERATE_PREV,
  reason_code_SOM_ITERATE_LAST
};

typedef struct som_object_flags
{
  unsigned int type:4;
  unsigned int is_armeabihf:1;

  /* True if the data segment should be forced to start at 8 byte alignment,
     false if it should be forced to start at 4 byte alignment (and not 8).  */
  unsigned int data_seg_8byte_aligned:1;

} som_object_flags;

typedef struct _som_object som_object;
struct _som_object
{
  /* Handle of object (currently base address of object, 0x8000 for app,
     0 = invalid).  */
  som_handle handle;

  /* Index of this object in the object/GOT array.  */
  int index;

  /* Pointer to start of library's address space.  */
  som_PTR text_segment;

  /* Size of read only text segment in bytes.  */
  unsigned text_size;

  /* Pointer to end of library's address space.  */
  som_PTR end_addr;

  /* Pointer to library's data segment. This is the client's copy of the data
     segment which is read/written to during program execution.  */
  som_PTR data_rw_segment;

  /* Pointer to the library data segment that is used once per client
     during initialisation. This is a read only copy of the data segment from the ELF file. */
  som_PTR data_ro_segment;

  /* Size of the read/write data segment.  */
  unsigned data_size;

  /* Pointer to library's GOT.  */
  som_PTR got_addr;

  /* Pointer to library's bss area.  */
  som_PTR bss_addr;

  /* Size of bss area.  */
  unsigned bss_size;

  /* Address of dynamic segment.  */
  som_PTR dynamic_addr;

  /* Size of dynamic segment.  */
  unsigned dynamic_size;

  som_object_flags flags;

  /* This member must be the last one present in the structure.  */
  char name[];
};

/* A link in the global list of libraries that are currently loaded.  */
typedef struct _som_library_object
{
  link_hdr link;

  /* Number of clients that are using this library.  */
  int usage_count;

  /* The time (roughly) at which the library will be removed from
     memory if it's usage count doesn't increase above 0 before
     hand.  */
  unsigned int expire_time;

  /* The object itself. This must be the last member of the structure
     to accommodate the name member of som_object.  */
  som_object object;

} som_library_object;
LINKLIST_ACCESS_FUNCTIONS (som_library_object)

/* A link in the private list of libraries that the client uses.
   If this structure is changed, then som_handlers.s needs changing too.  */
typedef struct _som_client_object
{
  link_hdr link;

#if USE_MAPPED_LIBRARIES
  /* Two pointers that define the start and end of the library's unmapped
     data segment. Used by the data abort handler to determine if this library
     should be mapped.  */
  som_PTR virtual_data_seg_start;
  som_PTR virtual_data_seg_end;
#endif

  /* The client's copy of the library's R/W segment.  */
  som_PTR data_segment;

  /* Pointer to the client's copy of the GOT.  */
  som_PTR got_addr;

  /* Pointer to the library itself.  */
  som_library_object *library;

#if USE_MAPPED_LIBRARIES
  /* An array storing the page mappings required to map the client data segment in and
     to map it out. The array is divided in two, the first half pointed to by "map"
     and the second half pointed to by "unmap". "map" is used to manage the allocation,
     "unmap" is a convenience variable so we don't have to work out the centre of the
     array.
     Created once when the library is registered for the client. Suitable
     for passing straight to OS_DynamicArea,22 */
  pmp_log_page_entry *map;
  pmp_log_page_entry *unmap;
  unsigned num_pages;

  bool mapped;
#endif
} som_client_object;
LINKLIST_ACCESS_FUNCTIONS (som_client_object)

#if USE_MAPPED_LIBRARIES
typedef struct abort_handler_block
{
  som_client_object *list;
  int da_number;

} abort_handler_block;

typedef struct filter_block
{
  som_client_object *list;
  int da_number;

} filter_block;
#endif

/* A link in the list of known clients.  */
typedef struct _som_client som_client;
struct _som_client
{
  link_hdr link;

  /* ID code for this client.  */
  som_client_ID unique_ID;

  /* Ordered (by base addr) linked list of libraries used by this client.
     First object is for the client itself.  */
  link_list object_list;

  /* An array of GOT pointers used by the PIC register loading instruction
     sequence. There is one entry for each object in the system. Each object
     is assigned an index (stored within its workspace) into this array. If the
     current client doesn't use an object then its entry is NULL.  */
  som_array gott_base;

  /* An array of _som_rt_elem structures. Used by the dynamic linker for fast
     access to relocation data.  */
  som_array runtime_array;

  struct
  {
    /* TRUE if the client's libraries are mapped in.  */
#if USE_MAPPED_LIBRARIES
    bool mapped:1;
#endif
    bool is_armeabihf:1;
  } flags;
  
  /* An object to record the details of the client. This must be the last
     member of the structure to accommodate the name member of som_object.  */
  som_object object;

#if USE_MAPPED_LIBRARIES
  /* Block of data passed to the abort handler in r12.  */
  abort_handler_block abort_handler_data;
#endif
};
LINKLIST_ACCESS_FUNCTIONS (som_client)

typedef struct _som_objinfo som_objinfo;
struct _som_objinfo
{
  som_PTR text_segment;

  unsigned text_size;

  /* Pointer to the library data segment in its position after the text segment.  */
  som_PTR library_data_segment;

  /* Pointer to the library data segment that is used for client initialisation.  */
  som_PTR library_init_segment;

  /* The client's copy of the r/w segment.  */
  som_PTR client_data_segment;

  /* Size of the r/w segment.  */
  unsigned data_size;

  /* Offset in bytes of the GOT into the r/w segment.  */
  unsigned got_offset;

  /* Offset in bytes of the bss area into the r/w segment.  */
  unsigned bss_offset;

  /* Size in bytes of the bss area.  */
  unsigned bss_size;

  /* Offset in bytes of dynamic segment into R/W segment.  */
  unsigned dyn_offset;

  /* Size in bytes of dynamic segment.  */
  unsigned dyn_size;

  /* Pointer to name of object.  */
  char *name;

  som_object_flags flags;
};

typedef struct _som_globals som_globals;
struct _som_globals
{
  /* Dynamic area for general data allocations (not library code).  */
  dynamic_area_block data_da;

  /* Dynamic area for paged memory allocation (32bit OS only).  */
  armeabi_allocator_t memory_page_allocator;

  /* List of clients known to system.  */
  link_list client_list;

  /* List of libraries loaded by system.  */
  link_list object_list;

  /* History list of recently deregistered clients.  */
  link_list client_history;

  /* Array of som_object pointers. A NULL pointer inidicates that an object
     expired and that this slot can be reused. NOT ARM_AAPCS */
  som_array object_array;

  /* Number of centiseconds a library is idle in memory before being
     removed.  */
  unsigned int max_idle_time;

  /* Number of centiseconds the garbage collection ticker occurs.  */
  unsigned int call_every_cs_delay;

#if USE_MAPPED_LIBRARIES
  filter_block filter_data;
#endif

  struct
  {
     bool no_client_check:1;
     bool host_32bit:1;
     bool callback_pending:1;
     bool call_every_enabled:1;
#if USE_MAPPED_LIBRARIES
     bool pre_filter_installed:1;
     bool post_filter_installed:1;
#endif
     bool writable_text:1;
  } flags;
};

extern som_globals global;

/* Return the client object whose ID is given.  */
extern som_client *som_find_client (som_handle ID);

extern _kernel_oserror *som_start_call_every (void *pw);
extern _kernel_oserror *som_stop_call_every (void *pw);
#if USE_MAPPED_LIBRARIES
extern _kernel_oserror *som_install_pre_filter (void *pw);
extern _kernel_oserror *som_remove_pre_filter (void *pw);
extern _kernel_oserror *som_install_post_filter (void *pw);
extern _kernel_oserror *som_remove_post_filter (void *pw);
extern void som_pre_wimp_filter(void);
extern void som_post_wimp_filter(void);
extern int som_abort_handler(void *);
#endif

/* Return the current client's structure.  */
static inline som_client *
FIND_CLIENT (void)
{
  return (som_client *) rt_workspace_get (rt_workspace_CLIENT_STRUCT_PTR);
}

static inline som_PTR
word_align (som_PTR addr)
{
  return (som_PTR) (((unsigned int) addr + 3) & ~3);
}

static inline bool
client_is_armeabihf (som_client *client)
{
  return client->flags.is_armeabihf;
}

#endif
