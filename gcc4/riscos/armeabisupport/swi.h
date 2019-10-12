/* swis.h
 *
 * Inline assembler versions and declarations of some common SWIs.
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef ARMEABISUPPORT_SWIS_H
#define ARMEABISUPPORT_SWIS_H

#include <swis.h>
#include "types.h"

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

_kernel_oserror *dynamic_area_create (const char *name,
				      int max_size,
				      bool needs_mapping,
				      dynamicarea_block *ret);

#ifndef USE_INLINE_SWIS
_kernel_oserror *rma_claim (int __size, void **block_ret);
_kernel_oserror *rma_free (void *_block);
_kernel_oserror *dynamic_pmp_claim_release (const int da,
					    pmp_phy_page_entry *page_list,
					    int num_entries);
_kernel_oserror *dynamic_pmp_map_unmap (const int da,
					pmp_log_page_entry *page_list,
					int num_entries);
_kernel_oserror *dynamic_pmp_resize (const int da,
				     int page_diff,
				     int *changed_ret);
_kernel_oserror *dynamic_pmp_page_info(const int da,
				       pmp_page_info_entry *page_list,
				       int num_entries);
void report_text(const char *text);
_kernel_oserror *heap_init(void *base, size_t size);
_kernel_oserror *heap_claim (void *base, size_t size, void **block_ret);
_kernel_oserror *heap_release (void *base, void *block);
_kernel_oserror *heap_extend (void *base, size_t by);
_kernel_oserror *heap_extend_block (void *base, void **block, size_t by);
_kernel_oserror *heap_block_size (void *base, void *block, size_t *size);
_kernel_oserror *dynamic_area_remove (unsigned da_number);
_kernel_oserror *dynamic_area_extend (unsigned da_number, size_t by);
_kernel_oserror *filter_register_pre_filter (const char *filter_name,
					     void (*filter_code)(void),
					     void *pw,
					     unsigned task_handle);
_kernel_oserror *filter_register_post_filter (const char *filter_name,
					      void (*filter_code)(void),
					      void *pw,
					      unsigned task_handle);
_kernel_oserror *filter_deregister_pre_filter (const char *filter_name,
					       void (*filter_code)(void),
					       void *pw,
					       unsigned task_handle);
_kernel_oserror *filter_deregister_post_filter (const char *filter_name,
						void (*filter_code)(void),
						void *pw,
						unsigned task_handle);
_kernel_oserror *get_app_base(unsigned *base_ret);
#else
enum
{
  reason_code_OSMODULE_CLAIM = 6,
  reason_code_OSMODULE_FREE
};

static inline _kernel_oserror *
rma_claim (int __size, void **block_ret)
{
  register unsigned reason asm("r0") = reason_code_OSMODULE_CLAIM;
  register void *block asm("r2");
  register unsigned size asm("r3") = __size;

  _kernel_oserror *err;

  asm volatile ("	SWI	%[XOS_Module];\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[err], #0;\n"
		: [err] "=r" (err),
		  [block] "=r" (block)
		: [size] "r" (size),
		  [reason] "r" (reason),
		  [XOS_Module] "i" (XOS_Bit | OS_Module)
		: "lr", "cc");

  if (block_ret && !err)
    *block_ret = block;

  return err;
}

static inline _kernel_oserror *
rma_free (void *_block)
{
  register unsigned reason asm("r0") = reason_code_OSMODULE_FREE;
  register _kernel_oserror *err asm("r0");
  register void *block asm("r2") = _block;

  asm volatile ("	SWI	%[XOS_Module];\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[err], #0;\n"
		: [err] "=r" (err)
		: [block] "r" (block),
		  [reason] "r" (reason),
		  [XOS_Module] "i" (XOS_Bit | OS_Module)
		: "lr", "cc");
  return err;
}

enum
{
  reason_code_DYNAMIC_AREA_CREATE,
  reason_code_DYNAMIC_AREA_REMOVE,
  reason_code_DYNAMIC_AREA_INFO,
  reason_code_DYNAMIC_AREA_ENUM,
  reason_code_DYNAMIC_AREA_RENUMBER,
  reason_code_DYNAMIC_AREA_FREE_SPACE
};


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
heap_claim (void *base_addr, int size, void **block_ret)
{
  _kernel_oserror *err;
  void *block;

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
heap_release (void *base_addr, void *block)
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
heap_extend (void *base_addr, int by)
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
heap_extend_block (void *base_addr, void **block, int by)
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
heap_block_size (void *base_addr, void *block, int *size)
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
			   pmp_phy_page_entry *page_list,
			   int num_entries)
{
  register unsigned reason asm("r0") = 21;
  register const int da asm("r1") = __da;

  _kernel_oserror *err;

  asm volatile ("	MOV	r2, %[page_list];\n"
		"	MOV	r3, %[num_entries];\n"
		"	SWI	%[XOS_DynamicArea];\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[err], #0;\n"
		: [err] "=r" (err)
		: "r" (da),
		  "r" (reason),
		  [num_entries] "Ir" (num_entries),
		  [page_list] "r" (page_list),
		  [XOS_DynamicArea] "i" (XOS_Bit | OS_DynamicArea)
		: "r2", "r3", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
dynamic_pmp_map_unmap (const int __da,
		       pmp_log_page_entry *page_list,
		       int num_entries)
{
  register unsigned reason asm("r0") = 22;
  register const int da asm("r1") = __da;

  _kernel_oserror *err;

  asm volatile ("	MOV	r2, %[page_list];\n"
		"	MOV	r3, %[num_entries];\n"
		"	SWI	%[XOS_DynamicArea];\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[err], #0;\n"
		: [err] "=r" (err)
		: "r" (da), "r" (reason),
		  [num_entries] "Ir" (num_entries),
		  [page_list] "r" (page_list),
		  [XOS_DynamicArea] "i" (XOS_Bit | OS_DynamicArea)
		: "r2", "r3", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
dynamic_pmp_resize (const int __da,
		    int __page_diff,
		    int *changed_ret)
{
  register const int reason asm("r0") = 23;
  register const int da asm("r1") = __da;
  register int page_diff asm("r2") = __page_diff;

  _kernel_oserror *err;

  asm volatile ("	SWI	%[XOS_DynamicArea];\n"
		"	MOV	%[changed_ret], r2;\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err), "+r" (page_diff), [changed_ret] "=r" (*changed_ret)
		: "r" (da), "r" (reason),
		  [XOS_DynamicArea] "i" (XOS_Bit | OS_DynamicArea)
		: "lr", "cc");
  return err;
}

static inline _kernel_oserror *
dynamic_pmp_page_info(const int __da,
		      pmp_page_info_entry *__page_list,
		      int __num_entries)
{
  register const int reason asm("r0") = 25;
  register const int da asm("r1") = __da;
  register pmp_page_info_entry *page_list asm("r2") = __page_list;
  register int num_entries asm("r3") = __num_entries;

  _kernel_oserror *err;

  asm volatile ("	SWI	%[XOS_DynamicArea];\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[err], #0;\n"
		: [err] "=r" (err), "+r" (page_list)
		: "r" (da), "r" (reason), "Ir" (num_entries),
		  [XOS_DynamicArea] "i" (XOS_Bit | OS_DynamicArea)
		: "memory", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
filter_register_pre_filter (const char *__filter_name,
			    void (*__filter_code)(void),
			    void *__pw,
			    unsigned __task_handle)
{
  register const char *filter_name asm("r0") = __filter_name;
  register void (*filter_code)(void) asm("r1") = __filter_code;
  register void *pw asm("r2") = __pw;
  register unsigned task_handle asm("r3") = __task_handle;
  _kernel_oserror *err;

  asm volatile ("	SWI	%[XOS_FilterRegisterPreFilter];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (filter_name),
		  "r" (filter_code),
		  "r" (pw),
		  "Ir" (task_handle),
		  [XOS_FilterRegisterPreFilter] "i" (XOS_Bit | Filter_RegisterPreFilter)
		: "lr", "cc");
  return err;
}

static inline _kernel_oserror *
filter_register_post_filter (const char *__filter_name,
			     void (*__filter_code)(void),
			     void *__pw,
			     unsigned __task_handle,
			     unsigned __poll_mask)
{
  register const char *filter_name asm("r0") = __filter_name;
  register void (*filter_code)(void) asm("r1") = __filter_code;
  register void *pw asm("r2") = __pw;
  register unsigned task_handle asm("r3") = __task_handle;
  register unsigned poll_mask asm("r4") = __poll_mask;
  _kernel_oserror *err;

  asm volatile ("	SWI	%[XOS_FilterRegisterPostFilter];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (filter_name),
		  "r" (filter_code),
		  "r" (pw),
		  "Ir" (task_handle),
		  "Ir" (poll_mask),
		  [XOS_FilterRegisterPostFilter] "i" (XOS_Bit | Filter_RegisterPostFilter)
		: "lr", "cc");
  return err;
}

static inline _kernel_oserror *
filter_deregister_pre_filter (const char *__filter_name,
			      void (*__filter_code)(void),
			      void *__pw,
			      unsigned __task_handle)
{
  register const char *filter_name asm("r0") = __filter_name;
  register void (*filter_code)(void) asm("r1") = __filter_code;
  register void *pw asm("r2") = __pw;
  register unsigned task_handle asm("r3") = __task_handle;
  _kernel_oserror *err;

  asm volatile ("	SWI	%[XOS_FilterDeRegisterPreFilter];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (filter_name),
		  "r" (filter_code),
		  "r" (pw),
		  "Ir" (task_handle),
		  [XOS_FilterDeRegisterPreFilter] "i" (XOS_Bit | Filter_DeRegisterPreFilter)
		: "lr", "cc");
  return err;
}

static inline _kernel_oserror *
filter_deregister_post_filter (const char *__filter_name,
			       void (*__filter_code)(void),
			       void *__pw,
			       unsigned __task_handle,
			       unsigned __poll_mask)
{
  register const char *filter_name asm("r0") = __filter_name;
  register void (*filter_code)(void) asm("r1") = __filter_code;
  register void *pw asm("r2") = __pw;
  register unsigned task_handle asm("r3") = __task_handle;
  register unsigned poll_mask asm("r4") = __poll_mask;
  _kernel_oserror *err;

  asm volatile ("	SWI	%[XOS_FilterDeRegisterPostFilter];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (filter_name),
		  "r" (filter_code),
		  "r" (pw),
		  "Ir" (task_handle),
		  "Ir" (poll_mask),
		  [XOS_FilterDeRegisterPostFilter] "i" (XOS_Bit | Filter_DeRegisterPostFilter)
		: "lr", "cc");
  return err;
}

static inline _kernel_oserror *
get_app_base(unsigned *base_ret)
{
  unsigned _block[] =  { 0, 0x8000, 0 };
  register unsigned reason_and_flags asm("r0") = (1 << 9) | (1 << 13);
  register unsigned *block asm("r1") = _block;
  register unsigned num_entries asm("r2") = 1;

  _kernel_oserror *err;

  asm volatile ("	SWI	%[XOS_Memory];\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[err], #0;\n"
		: [err] "=r" (err), "+r" (block)
		: "r" (reason_and_flags), "r" (num_entries),
		  [XOS_Memory] "i" (XOS_Bit | OS_Memory)
		: "lr", "cc", "memory");

  *base_ret = block[2];

  return err;
}
#endif

#endif
