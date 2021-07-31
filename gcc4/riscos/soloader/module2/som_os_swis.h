/* som_os_swis.h
 *
 * Inline assembler versions of some common OS SWIs.
 *
 * Copyright 2007-2019 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_OS_SWIS_H
#define SOM_OS_SWIS_H

#include <swis.h>
#include <stdlib.h>
#include <string.h>
#include "som_types.h"

/* Memory mapping SWIs. */
#define PMP_PAGE_RELEASE		(-1)
#define PMP_PAGE_UNMAP			(-1)
#define PMP_PAGE_KERNEL_CHOICE		(-2)

/* Memory access values for privileged/unprivilileged modes.  */
#define PMP_MEM_ACCESS_RWE_RWE		0
#define PMP_MEM_ACCESS_RWE_RE		1
#define PMP_MEM_ACCESS_RWE_NONE		2
#define PMP_MEM_ACCESS_RE_RE		3

#define ARMEABISUPPORT_ABORTOP_REGISTER		0
#define ARMEABISUPPORT_ABORTOP_DEREGISTER	1
#define ARMEABISUPPORT_ABORTOP_INSTALL		2
#define ARMEABISUPPORT_ABORTOP_REMOVE		3

#define ARMEABISUPPORT_MEMORYOP_NEW_ALLOCATOR		0
#define ARMEABISUPPORT_MEMORYOP_DESTROY_ALLOCATOR 	1
#define ARMEABISUPPORT_MEMORYOP_ALLOC			2
#define ARMEABISUPPORT_MEMORYOP_CLAIM_PAGES		3
#define ARMEABISUPPORT_MEMORYOP_RELEASE_PAGES		4
#define ARMEABISUPPORT_MEMORYOP_MAP_PAGES		5
#define ARMEABISUPPORT_MEMORYOP_UNMAP_PAGES		6
#define ARMEABISUPPORT_MEMORYOP_FREE			7
#define ARMEABISUPPORT_MEMORYOP_ALLOCATOR_INFO		8
#define ARMEABISUPPORT_MEMORYOP_GET_PAGE_NUMBER		9
#define ARMEABISUPPORT_MEMORYOP_ALLOCATOR_FROM_ADDRESS	10

#define ARMEABISUPPORT_ALLOCATOR_TYPE_MASK		0xF
#define ARMEABISUPPORT_ALLOCATOR_TYPE_PAGE_MAPPED	1
#define ARMEABISUPPORT_ALLOCATOR_TYPE_HEAP		2
#define ARMEABISUPPORT_ALLOCATOR_FLAG_GLOBAL		(1 << 5)

#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAG_ALLOC_ONLY	(0 << ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAG_CLAIM	(1 << ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAG_MAP		(2 << ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_MASK	(3 << ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT	16

typedef void *armeabi_allocator_t;

typedef struct mem_map_entry
{
  int page_number;		/* Signed so that array can be terminated by -1. */
  void *address;
  unsigned int protection;
} mem_map_entry;

typedef struct pmp_phy_page_entry
{
  int pmp_page_index;
  int physical_page_number;
  unsigned flags;
} pmp_phy_page_entry;

typedef struct pmp_log_page_entry
{
  int da_page_number;
  int pmp_page_index;
  unsigned flags;
} pmp_log_page_entry;

typedef struct pmp_page_info_entry
{
  int pmp_page_index;
  int physical_page_number;
  int da_page_number;
  unsigned flags;
} pmp_page_info_entry;

static inline _kernel_oserror *
os_find_mem_map_entries (mem_map_entry *mem_map_array)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[mem_map_array];\n\t"
		"SWI	%[os_find_mem_map_entries];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [mem_map_array] "r" (mem_map_array),
		  [os_find_mem_map_entries] "i" (XOS_Bit | OS_FindMemMapEntries)
		: "r0", "r14", "cc");
  return err;
}

static inline _kernel_oserror *
os_set_mem_map_entries (mem_map_entry *mem_map_array)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[mem_map_array];\n\t"
		"SWI	%[os_set_mem_map_entries];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [mem_map_array] "r" (mem_map_array),
		  [os_set_mem_map_entries] "i" (XOS_Bit | OS_SetMemMapEntries)
		: "r0", "r14", "cc");
  return err;
}

/* Dynamic Area SWIs.  */
enum
{
  reason_code_DYNAMIC_AREA_CREATE,
  reason_code_DYNAMIC_AREA_REMOVE,
  reason_code_DYNAMIC_AREA_INFO,
  reason_code_DYNAMIC_AREA_ENUM,
  reason_code_DYNAMIC_AREA_RENUMBER,
  reason_code_DYNAMIC_AREA_FREE_SPACE
};

typedef struct dynamic_area_block
{
  unsigned int number;
  som_PTR base_addr;
  som_PTR end_addr;
} dynamic_area_block;

extern _kernel_oserror *dynamic_area_create (const char *name, int init_size,
					     int max_size,
					     dynamic_area_block *ret);

static inline _kernel_oserror *
dynamic_area_remove (unsigned int da_number)
{
  _kernel_oserror *err;

  if (da_number == -1)
    return NULL;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[da_number];\n\t"
		"SWI	%[os_dynamic_area];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [da_number] "r" (da_number),
		  [os_dynamic_area] "i" (XOS_Bit | OS_DynamicArea),
		  [reason] "I" (reason_code_DYNAMIC_AREA_REMOVE)
		: "r0", "r1", "r14", "cc");
  return err;
}

static inline _kernel_oserror *
dynamic_area_extend (unsigned int da_number, int by)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[da_number];\n\t"
		"MOV	r1, %[by];\n\t"
		"SWI	%[os_change_dynamic_area];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [da_number] "r" (da_number),
		  [by] "r" (by),
		  [os_change_dynamic_area] "i" (XOS_Bit | OS_ChangeDynamicArea)
		: "r0", "r1", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
dynamic_pmp_claim_release (const int __da,
			   pmp_phy_page_entry *__page_list,
			   int __num_entries)
{
  register const int da asm("r1") = __da;
  register pmp_phy_page_entry *page_list asm("r2") = __page_list;
  register int num_entries asm("r3") = __num_entries;

  _kernel_oserror *err;

  asm volatile ("	MOV	r0, #21;\n"
		"	SWI	%[os_dynamic_area];\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[err], #0;\n"
		: [err] "=r" (err), "+r" (page_list), "+r" (num_entries)
		: "r" (da),
		  [os_dynamic_area] "i" (XOS_Bit | OS_DynamicArea)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
dynamic_pmp_map_unmap (const int __da,
		       pmp_log_page_entry *__page_list,
		       int __num_entries)
{
  register const int da asm("r1") = __da;
  register pmp_log_page_entry *page_list asm("r2") = __page_list;
  register int num_entries asm("r3") = __num_entries;

  _kernel_oserror *err;

  asm volatile ("	MOV	r0, #22;\n"
		"	SWI	%[os_dynamic_area];\n"
		" bvc 1f;\n"
		" add r0, r0, #4;\n"
		" swi 0x54C80;\n"
		" sub r0, r0, #4;\n"
		"1:\n"
		" teq r3,#0;\n"
		" beq 0f;\n"
		" swi 0x74c81;\n"
		".asciz \"** dynamic_pmp_map_unmap: r3 not 0\";\n"
		".align;\n"
		"0:\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[err], #0;\n"
		: [err] "=r" (err), "+r" (page_list), "+r" (num_entries)
		: "r" (da),
		  [os_dynamic_area] "i" (XOS_Bit | OS_DynamicArea)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
dynamic_pmp_resize (const int __da,
		    int __page_diff,
		    int *changed_ret)
{
  register const int da asm("r1") = __da;
  register int page_diff asm("r2") = __page_diff;

  _kernel_oserror *err;

  asm volatile ("	MOV	r0, #23;\n"
		"	SWI	%[os_dynamic_area];\n"
		"	MOV	%[changed_ret], r2;\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err), "+r" (page_diff), [changed_ret] "=r" (*changed_ret)
		: "r" (da),
		  [os_dynamic_area] "i" (XOS_Bit | OS_DynamicArea)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
dynamic_pmp_page_info(const int __da,
		      pmp_page_info_entry *__page_list,
		      int __num_entries)
{
  register const int da asm("r1") = __da;
  register pmp_page_info_entry *page_list asm("r2") = __page_list;
  register int num_entries asm("r3") = __num_entries;

  _kernel_oserror *err;

  asm volatile ("	MOV	r0, #25;\n"
		"	SWI	%[os_dynamic_area];\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[err], #0;\n"
		: [err] "=r" (err), "+r" (page_list), "+r" (num_entries)
		: "r" (da),
		  [os_dynamic_area] "i" (XOS_Bit | OS_DynamicArea)
		: "r0", "lr", "cc");
  return err;
}

		      
/* Heap SWIs.  */
enum
{
  reason_code_HEAP_INIT,
  reason_code_HEAP_DESCRIBE,
  reason_code_HEAP_CLAIM,
  reason_code_HEAP_RELEASE,
  reason_code_HEAP_CHANGE_BLOCK_SIZE,
  reason_code_HEAP_CHANGE_HEAP_SIZE,
  reason_code_HEAP_READ_BLOCK_SIZE
};

static inline _kernel_oserror *
heap_init (void *base_addr, int size)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[base_addr];\n\t"
		"MOV	r3, %[size];\n\t"
		"SWI	%[os_heap];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [base_addr] "r" (base_addr),
		  [size] "rI" (size),
		  [os_heap] "i" (XOS_Bit | OS_Heap),
		  [reason] "I" (reason_code_HEAP_INIT)
		: "r0", "r1", "r2", "r14", "cc");
  return err;
}

static inline _kernel_oserror *
heap_claim (void *base_addr, int size, som_PTR *block_ret)
{
  _kernel_oserror *err;
  som_PTR block;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[base_addr];\n\t"
		"MOV	r3, %[size];\n\t"
		"SWI	%[os_heap];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		"MOVVS	%[block], #0;\n\t"
		"MOVVC	%[block], r2;\n\t"
		: [err] "=r" (err), [block] "=r" (block)
		: [base_addr] "r" (base_addr),
		  [size] "rI" (size),
		  [os_heap] "i" (XOS_Bit | OS_Heap),
		  [reason] "I" (reason_code_HEAP_CLAIM)
		: "r0", "r1", "r2", "r3", "r14", "cc");

  if (block_ret)
    *block_ret = block;

  return err;
}

static inline _kernel_oserror *
heap_release (som_PTR base_addr, som_PTR block)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[base_addr];\n\t"
		"MOV	r2, %[block];\n\t"
		"SWI	%[os_heap];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [base_addr] "r" (base_addr),
		  [block] "r" (block),
		  [os_heap] "i" (XOS_Bit | OS_Heap),
		  [reason] "I" (reason_code_HEAP_RELEASE)
		: "r0", "r1", "r2", "r14", "cc");
  return err;
}

static inline _kernel_oserror *
heap_extend (som_PTR base_addr, int by)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[base_addr];\n\t"
		"MOV	r3, %[by];\n\t"
		"SWI	%[os_heap];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [base_addr] "r" (base_addr),
		  [by] "rI" (by),
		  [os_heap] "i" (XOS_Bit | OS_Heap),
		  [reason] "I" (reason_code_HEAP_CHANGE_HEAP_SIZE)
		: "r0", "r1", "r2", "r3", "r14", "cc");
  return err;
}

static inline _kernel_oserror *
heap_extend_block (som_PTR base_addr, som_PTR *block, int by)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[base_addr];\n\t"
		"LDR	r2, %[block];\n\t"
		"MOV	r3, %[by];\n\t"
		"SWI	%[os_heap];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		"STRVC	r2, %[block];\n\t"
		: [err] "=r" (err),
		  [block] "+m" (*block)
		: [base_addr] "r" (base_addr),
		  [by] "rI" (by),
		  [os_heap] "i" (XOS_Bit | OS_Heap),
		  [reason] "I" (reason_code_HEAP_CHANGE_BLOCK_SIZE)
		: "r0", "r1", "r2", "r3", "r14", "cc", "memory");
  return err;
}

static inline _kernel_oserror *
heap_block_size (som_PTR base_addr, som_PTR block, int *size)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[base_addr];\n\t"
		"MOV	r2, %[block];\n\t"
		"SWI	%[os_heap];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		"STRVC	r3, %[size];\n\t"
		: [err] "=r" (err),
		  [size] "=m" (*size)
		: [base_addr] "r" (base_addr),
		  [block] "r" (block),
		  [os_heap] "i" (XOS_Bit | OS_Heap),
		  [reason] "I" (reason_code_HEAP_READ_BLOCK_SIZE)
		: "r0", "r1", "r2", "r3", "r14", "cc", "memory");
  return err;

}

/* OS_Module SWIs.  */

enum
{
  reason_code_OSMODULE_CLAIM = 6,
  reason_code_OSMODULE_FREE
};

static inline _kernel_oserror *
RMA_claim (int size, som_PTR *block_ret)
{
  _kernel_oserror *err;
  som_PTR block;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r3, %[size];\n\t"
		"SWI	%[os_module];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		"MOVVS	%[block], #0;\n\t"
		"MOVVC	%[block], r2;\n\t"
		: [err] "=r" (err),
		  [block] "=r" (block)
		: [size] "r" (size),
		  [os_module] "i" (XOS_Bit | OS_Module),
		  [reason] "I" (reason_code_OSMODULE_CLAIM)
		: "r0", "r1", "r2", "r3", "lr", "cc");

  if (block_ret)
    *block_ret = block;

  return err;
}

static inline _kernel_oserror *
RMA_free (som_PTR block)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r2, %[block];\n\t"
		"SWI	%[os_module];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [block] "r" (block),
		  [os_module] "i" (XOS_Bit | OS_Module),
		  [reason] "I" (reason_code_OSMODULE_FREE)
		: "r0", "r1", "r2", "lr", "cc");
  return err;
}

/* DDEUtils.  */

static inline unsigned int
ddeutils_is_present (void)
{
  static const char ddeutil_swi_name[] = "DDEUtils_GetCl";
  unsigned int res;

  asm volatile ("MOV	r1, %[name];\n\t"
		"SWI	%[os_swi_number_from_string];\n\t"
		"MOVVS	%[res], #0;\n\t"
		"MOVVC	%[res], #1;\n\t"
		: [res] "=r" (res)
		: [name] "r" (ddeutil_swi_name),
		  [os_swi_number_from_string] "i" (XOS_Bit | OS_SWINumberFromString)
		: "r0", "r1", "lr", "cc");
  return res;
}

static inline size_t
ddeutils_get_cl_size (void)
{
  size_t res;

  /* According to Desktop Tools P.192, doesn't return any errors.  */
  asm volatile ("SWI	%[ddeutils_get_cl_size];\n\t"
		"MOVVC	%[res], r0;\n\t"
		"MOVVS	%[res], #0;\n\t"
		: [res] "=r" (res)
		: [ddeutils_get_cl_size] "i" (XOS_Bit | DDEUtils_GetCLSize)
		: "r0", "lr", "cc");
  return res;
}

static inline void
ddeutils_get_cl (char *buffer)
{
  asm volatile ("MOV	r0, %[buffer];\n\t"
		"SWI	%[ddeutils_get_cl];\n\t"
		:	/* no outputs */
		: [buffer] "r" (buffer),
		  [ddeutils_get_cl] "i" (XOS_Bit | DDEUtils_GetCl)
		: "r0", "lr", "cc");
}

/* Automatically calls SWI "DDEUtils_SetCLSize" first.  */
static inline void
ddeutils_set_cl (const char *tail)
{
  int len = strlen (tail) + 1;

  asm volatile ("MOV	r0, %[len];\n\t"
		"SWI	%[ddeutils_set_cl_size];\n\t"
		"MOV	r0, %[tail];\n\t"
		"SWI	%[ddeutils_set_cl];\n\t"
		:	/* no outputs */
		: [len] "r" (len),
		  [tail] "r" (tail),
		  [ddeutils_set_cl_size] "i" (XOS_Bit | DDEUtils_SetCLSize),
		  [ddeutils_set_cl] "i" (XOS_Bit | DDEUtils_SetCL)
		: "r0", "lr", "cc");
}

/* General SWIs.  */

static inline unsigned int
os_read_monotonic_time (void)
{
  unsigned int t;

  asm volatile ("SWI	%[os_read_monotonic_time];\n\t"
		"MOV	%[t], r0;\n\t"
		: [t] "=r" (t)
		: [os_read_monotonic_time] "i" (XOS_Bit | OS_ReadMonotonicTime)
		: "r0", "lr", "cc");
  return t;
}

static inline _kernel_oserror *
os_call_every (unsigned int t, void (*handler) (void), void *pw)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[time];\n\t"
		"MOV	r1, %[handler];\n\t"
		"MOV	r2, %[pw];\n\t"
		"SWI	%[os_callevery];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [time] "r" (t),
		  [handler] "r" (handler),
		  [pw] "r" (pw),
		  [os_callevery] "i" (XOS_Bit | OS_CallEvery)
		: "r0", "r1", "r2", "r14", "cc");
  return err;
}

static inline _kernel_oserror *
os_remove_ticker_event (void (*handler) (void), void *pw)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[handler];\n\t"
		"MOV	r1, %[pw];\n\t"
		"SWI	%[os_removetickerevent];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [handler] "r" (handler),
		  [pw] "r" (pw),
		  [os_removetickerevent] "i" (XOS_Bit | OS_RemoveTickerEvent)
		: "r0", "r1", "r14", "cc");
  return err;
}

static inline void
os_add_callback (void (*handler) (void), void *pw)
{
  asm volatile ("MOV	r0, %[handler];\n\t"
		"MOV	r1, %[pw];\n\t"
		"SWI	%[os_add_callback];\n\t"
		:	/* no outputs */
		: [handler] "r" (handler),
		  [pw] "r" (pw),
		  [os_add_callback] "i" (XOS_Bit | OS_AddCallBack)
		: "r0", "r1", "r14", "cc");
}

static inline void
os_remove_callback (void (*handler) (void), void *pw)
{
  asm volatile ("MOV	r0, %[handler];\n\t"
		"MOV	r1, %[pw];\n\t"
		"SWI	%[os_remove_callback];\n\t"
		:	/* no outputs */
		: [handler] "r" (handler),
		  [pw] "r" (pw),
		  [os_remove_callback] "i" (XOS_Bit | OS_RemoveCallBack)
		: "r0", "r1", "r14", "cc");
}

/* s updated to point to terminating character.  */
static inline unsigned int
os_read_unsigned (const char **s, int base)
{
  unsigned int res;

  asm volatile ("MOV	r0, %[base];\n\t"
		"LDR	r1, %[s];\n\t"
		"SWI	%[os_read_unsigned];\n\t"
		"STR	r1, %[s];\n\t"
		"MOV	%[res], r2;\n\t"
		: [res] "=r" (res)
		: [base] "r" (base),
		  [s] "m" (*s),
		  [os_read_unsigned] "i" (XOS_Bit | OS_ReadUnsigned)
		: "r0", "r1", "r2", "lr", "cc", "memory");
  return res;
}

/* Inform RISC OS that a new app is starting by calling OS_FSControl,2.  */
static inline _kernel_oserror *
os_start_app (const char *com_tail, void *cao, const char *com_name)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, #2;\n\t"
		"MOV	r1, %[tail];\n\t"
		"MOV	r2, %[cao];\n\t"
		"MOV	r3, %[name];\n\t"
		"SWI	%[os_fscontrol];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [tail] "r" (com_tail),
		  [cao] "r" (cao),
		  [name] "r" (com_name),
		  [os_fscontrol] "i" (XOS_Bit | OS_FSControl)
		: "r0", "r1", "r2", "r3", "lr", "cc");
  return err;
}

typedef struct os_env_block
{
  char *command;
  unsigned int ram_limit;
  char *time;
} os_env_block;

static inline _kernel_oserror *
os_get_env (os_env_block *block)
{
  _kernel_oserror *err;

  asm volatile ("SWI	%[os_getenv];\n\t"
		"STRVC	r0, %[block0];\n\t"
		"STRVC	r1, %[block4];\n\t"
		"STRVC	r2, %[block8];\n\t"
		"MOVVC	%[err], #0;\n\t"
		"MOVVS	%[err], r0;\n\t"
		: [err] "=r" (err),
		  [block0] "=m" (block->command),
		  [block4] "=m" (block->ram_limit),
		  [block8] "=m" (block->time)
		: [os_getenv] "i" (XOS_Bit | OS_GetEnv)
		: "r0", "r1", "r2", "lr", "cc", "memory");
  return err;
}

/* Return the buffer size needed to the contents of the system variable
   'var_name' including the terminating character. When the system variable
   can not be found, size 1 is returned.  */
static inline int
os_read_var_val_size (const char *var_name)
{
  int size;

  asm volatile ("MOV	r0, %[name];\n\t"
		"MOV	r1, #0;\n\t"
		"MOV	r2, #-1;\n\t"
		"MOV	r3, #0;\n\t"
		"MOV	r4, #0;\n\t"
		"SWI	%[os_read_var_val];\n\t"
		"CMP	r2, #0;\n\t"
		"MOVEQ	%[size], #1;\n\t"
		"RSBNE	%[size], r2, #0;\n\t"
		: [size] "=r" (size)
		: [name] "r" (var_name),
		  [os_read_var_val] "i" (XOS_Bit | OS_ReadVarVal)
		: "r0", "r1", "r2", "r3", "r4", "lr", "cc");
  return size;
}

static inline _kernel_oserror *
os_read_var_val (const char *var_name, char *buffer, int buf_size)
{
  _kernel_oserror *err;

  asm volatile ("MOV	r0, %[name];\n\t"
		"MOV	r1, %[buffer];\n\t"
		"MOV	r2, %[buf_size];\n\t"
		"MOV	r3, #0;\n\t"
		"MOV	r4, #0;\n\t"
		"SWI	%[os_read_var_val];\n\t"
		"MOVVC	r0, #0;\n\t" /* Terminate the contents of variable.  */
		"STRVCB	r0, [r1, r2];\n\t"
		"MOVVC	%[err], #0;\n\t"
		"MOVVS	%[err], r0;\n\t"
		: [err] "=r" (err)
		: [name] "r" (var_name),
		  [buffer] "r" (buffer),
		  [buf_size] "r" (buf_size),
		  [os_read_var_val] "i" (XOS_Bit | OS_ReadVarVal)
		: "r0", "r1", "r2", "r3", "r4", "lr", "cc", "memory");

  return err;
}

static inline void
os_synchronise_code_area (const void *from, const void *to)
{
  asm volatile ("MOV	r0, #1;\n\t"
		"MOV	r1, %[from];\n\t"
		"MOV	r2, %[to];\n\t"
		"SWI	%[os_synchronise_code_area];\n\t"
		: /* No outputs */
		: [from] "r" (from),
		  [to] "r" (to),
		  [os_synchronise_code_area] "i" (XOS_Bit | OS_SynchroniseCodeAreas)
		: "r0", "r1", "r2", "lr", "cc");
}

static inline _kernel_oserror *
filter_register_pre_filter (const char *filter_name,
			    void (*filter_code)(void),
			    void *pw,
			    unsigned task_handle)
{
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[filter_name];\n"
		"	MOV	r1, %[filter_code];\n"
		"	MOV	r2, %[pw];\n"
		"	MOV	r3, %[task_handle];\n"
		"	SWI	%[filter_register_pre_filter];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: [filter_name] "r" (filter_name),
		  [filter_code] "r" (filter_code),
		  [pw] "r" (pw),
		  [task_handle] "r" (task_handle),
		  [filter_register_pre_filter] "i" (XOS_Bit | Filter_RegisterPreFilter)
		: "r0", "r1", "r2", "r3", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
filter_register_post_filter (const char *filter_name,
			     void (*filter_code)(void),
			     void *pw,
			     unsigned task_handle,
			     unsigned poll_mask)
{
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[filter_name];\n"
		"	MOV	r1, %[filter_code];\n"
		"	MOV	r2, %[pw];\n"
		"	MOV	r3, %[task_handle];\n"
		"	MOV	r4, %[poll_mask];\n"
		"	SWI	%[filter_register_post_filter];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: [filter_name] "r" (filter_name),
		  [filter_code] "r" (filter_code),
		  [pw] "r" (pw),
		  [task_handle] "r" (task_handle),
		  [poll_mask] "r" (poll_mask),
		  [filter_register_post_filter] "i" (XOS_Bit | Filter_RegisterPostFilter)
		: "r0", "r1", "r2", "r3", "r4", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
filter_deregister_pre_filter (const char *filter_name,
			      void (*filter_code)(void),
			      void *pw,
			      unsigned task_handle)
{
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[filter_name];\n"
		"	MOV	r1, %[filter_code];\n"
		"	MOV	r2, %[pw];\n"
		"	MOV	r3, %[task_handle];\n"
		"	SWI	%[filter_deregister_pre_filter];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: [filter_name] "r" (filter_name),
		  [filter_code] "r" (filter_code),
		  [pw] "r" (pw),
		  [task_handle] "r" (task_handle),
		  [filter_deregister_pre_filter] "i" (XOS_Bit | Filter_DeRegisterPreFilter)
		: "r0", "r1", "r2", "r3", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
filter_deregister_post_filter (const char *filter_name,
			       void (*filter_code)(void),
			       void *pw,
			       unsigned task_handle,
			       unsigned poll_mask)
{
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[filter_name];\n"
		"	MOV	r1, %[filter_code];\n"
		"	MOV	r2, %[pw];\n"
		"	MOV	r3, %[task_handle];\n"
		"	MOV	r4, %[poll_mask];\n"
		"	SWI	%[filter_deregister_post_filter];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: [filter_name] "r" (filter_name),
		  [filter_code] "r" (filter_code),
		  [pw] "r" (pw),
		  [task_handle] "r" (task_handle),
		  [poll_mask] "r" (poll_mask),
		  [filter_deregister_post_filter] "i" (XOS_Bit | Filter_DeRegisterPostFilter)
		: "r0", "r1", "r2", "r3", "r4", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_new_allocator(const char *__name,
			     unsigned __max_size,
			     unsigned __flags,
			     armeabi_allocator_t *handle_ret)
{
  register const char *name asm("r1") = __name;
  register unsigned max_size asm("r2") = __max_size;
  register unsigned flags asm("r3") = __flags;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVC	%[handle], r0;\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVS	%[handle], #0;\n"
		: [err] "=r" (err), [handle] "=r" (*handle_ret)
		: "r" (name),
		  "r" (max_size),
		  "r" (flags),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_NEW_ALLOCATOR)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_alloc(armeabi_allocator_t __allocator,
		     unsigned __num_pages,
		     unsigned __flags,
		     som_PTR *block_ret)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register unsigned num_pages asm("r2") = __num_pages;
  register unsigned flags asm("r3") = __flags;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVC	%[block], r0;\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVS	%[block], #0;\n"
		: [err] "=r" (err), [block] "=r" (*block_ret)
		: "r" (allocator),
		  "r" (num_pages),
		  "r" (flags),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_ALLOC)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_free(armeabi_allocator_t __allocator,
		    som_PTR __block)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register som_PTR block asm("r2") = __block;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (allocator),
		  "r" (block),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_FREE)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_destroy_allocator(armeabi_allocator_t __allocator)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (allocator),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_DESTROY_ALLOCATOR)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_claim_pages(armeabi_allocator_t __allocator,
			   som_PTR __block,
			   unsigned __num_pages)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register som_PTR block asm("r2") = __block;
  register unsigned num_pages asm("r3") = __num_pages;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (allocator),
		  "r" (block),
		  "r" (num_pages),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_CLAIM_PAGES)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_release_pages(armeabi_allocator_t __allocator,
			     som_PTR __block,
			     unsigned __num_pages)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register som_PTR block asm("r2") = __block;
  register unsigned num_pages asm("r3") = __num_pages;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (allocator),
		  "r" (block),
		  "r" (num_pages),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_RELEASE_PAGES)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_map_pages(armeabi_allocator_t __allocator,
			 som_PTR __block,
			 unsigned __num_pages,
			 unsigned __flags)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register som_PTR block asm("r2") = __block;
  register unsigned num_pages asm("r3") = __num_pages;
  register unsigned flags asm("r4") = __flags;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (allocator),
		  "r" (block),
		  "r" (num_pages),
		  "r" (flags),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_MAP_PAGES)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_unmap_pages(armeabi_allocator_t __allocator,
			   som_PTR __block,
			   unsigned __num_pages)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register som_PTR block asm("r2") = __block;
  register unsigned num_pages asm("r3") = __num_pages;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (allocator),
		  "r" (block),
		  "r" (num_pages),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_UNMAP_PAGES)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_info(armeabi_allocator_t __allocator,
		    int *da_number_ret,
		    som_PTR *base_ret,
		    som_PTR *end_ret)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[da_num], r0;\n"
		"	MOVVS	%[da_num], #-1;\n"
		"	MOV	%[base], r1;\n"
		"	MOV	%[end], r2;\n"
		: [err] "=r" (err),
		  [da_num] "=r" (*da_number_ret),
		  [base] "=r" (*base_ret),
		  [end] "=r" (*end_ret)
		: "r" (allocator),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_ALLOCATOR_INFO)
		: "r0", "r2", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_allocator_get_page_number(armeabi_allocator_t *__allocator,
					 som_PTR __addr,
					 int *page_number_ret)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register som_PTR addr asm("r2") = __addr;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[page], r0;\n"
		"	MOVVS	%[page], #-1;\n"
		: [err] "=r" (err), [page] "=r" (*page_number_ret)
		: "r" (allocator),
		  "r" (addr),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_GET_PAGE_NUMBER)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_allocator_from_address(som_PTR __addr,
				      armeabi_allocator_t *allocator_ret)
{
  register som_PTR addr asm("r1") = __addr;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[allocator], r0;\n"
		"	MOVVS	%[allocator], #0;\n"
		: [err] "=r" (err), [allocator] "=r" (*allocator_ret)
		: "r" (addr),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_ALLOCATOR_FROM_ADDRESS)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_abort_register(int (*handler)(void *fault_address), void *r12)
{
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	MOV	r1, %[handler];\n"
		"	MOV	r2, %[r12];\n"
		"	SWI	%[XARMEABISupport_AbortOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: [handler] "r" (handler),
		  [r12] "r" (r12),
		  [XARMEABISupport_AbortOp] "i" (XOS_Bit | ARMEABISupport_AbortOp),
		  [reason] "I" (ARMEABISUPPORT_ABORTOP_REGISTER)
		: "r0", "r1", "r2", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_abort_deregister(int (*handler)(void *fault_address), void *r12)
{
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	MOV	r1, %[handler];\n"
		"	MOV	r2, %[r12];\n"
		"	SWI	%[XARMEABISupport_AbortOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: [handler] "r" (handler),
		  [r12] "r" (r12),
		  [XARMEABISupport_AbortOp] "i" (XOS_Bit | ARMEABISupport_AbortOp),
		  [reason] "I" (ARMEABISUPPORT_ABORTOP_DEREGISTER)
		: "r0", "r1", "r2", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_install_abort_handler(void)
{
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_AbortOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: [XARMEABISupport_AbortOp] "i" (XOS_Bit | ARMEABISupport_AbortOp),
		  [reason] "I" (ARMEABISUPPORT_ABORTOP_INSTALL)
		: "r0", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_remove_abort_handler(void)
{
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_AbortOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: [XARMEABISupport_AbortOp] "i" (XOS_Bit | ARMEABISupport_AbortOp),
		  [reason] "I" (ARMEABISUPPORT_ABORTOP_REMOVE)
		: "r0", "lr", "cc");
  return err;
}

static __inline char *
hex_to_string(unsigned int v, char *buffer, int l)
{
char *term;

  __asm volatile (
#ifdef __TARGET_MODULE__
		  "	STR	lr, [sp, #-4]!;\n"
#endif
		  "	MOV	r0, %1;\n"
		  "	MOV	r1, %2;\n"
		  "	MOV	r2, %3;\n"
		  "	SWI	%[os_converthex8];\n"
		  "	MOV	%0, r1;\n"
#ifdef __TARGET_MODULE__
		  "	LDR	lr, [sp], #4;\n"
#endif
		  : "=r" (term)
		  : "r" (v), "r" (buffer), "rI" (l), [os_converthex8] "i" (0xD4)
		  : "a1", "a2", "a3", "r14", "cc");
  return term;
}

/* Signed 32 bit integer to string
 * return pointer to terminating null in buffer
 */
static __inline char *
sint32_to_string(int v, char *buffer, int l)
{
char *term;

  __asm volatile (
#ifdef __TARGET_MODULE__
		  "	STR	lr, [sp, #-4]!;\n"
#endif
		  "	MOV	r0, %1;\n"
		  "	MOV	r1, %2;\n"
		  "	MOV	r2, %3;\n"
		  "	SWI	%[os_convertint4];\n"
		  "	MOV	%0, r1;\n"
#ifdef __TARGET_MODULE__
		  "	LDR	lr, [sp], #4;\n"
#endif
		  : "=r" (term)
		  : "r" (v), "r" (buffer), "rI" (l), [os_convertint4] "i" (0xDC)
		  : "a1", "a2", "a3", "r14", "cc");
  return term;
}

static __inline void report_text(const char *__s)
{
  register const char *s asm("r0") = __s;

  asm volatile ("	STR	lr, [sp, #-4]!;\n"
		"	SWI	%[report_text0];\n"
		"	LDR	lr, [sp], #4;\n"
		: /* no outputs */
		: "r" (s), [report_text0] "i" (0x54C80)
		: "r14", "cc");
}

static __inline void print_text(const char *s)
{
  __asm volatile ("	STR	lr, [sp, #-4]!;\n"
		  "	MOV	r0, %[s];\n"
		  "	SWI	%[SWI_OS_Write0];\n"
		"	LDR	lr, [sp], #4;\n"
		  : /* no outputs */
		  : [s] "r" (s),
		    [SWI_OS_Write0] "i" (0x2)
		  : "a1", "r14", "cc");
}
static __inline void print_hex(unsigned int v)
{
  char buffer[12];
  hex_to_string(v, buffer, 12);
  print_text(buffer);
}

static __inline void print_dec(unsigned int v)
{
  char buffer[20];
  sint32_to_string(v, buffer, 20);
  print_text(buffer);
}

static __inline void print_nl(void)
{
  __asm volatile (
		  "	SWI	0x10a;\n"
		  "	SWI	0x10d;\n"
		  ::: "lr");
}

static __inline void report_hex(unsigned v)
{
  char buffer[12];
  hex_to_string(v, buffer, 12);
  report_text(buffer);
}

static __inline void report_dec(unsigned v)
{
  char buffer[12];
  sint32_to_string(v, buffer, 12);
  report_text(buffer);
}

#endif
