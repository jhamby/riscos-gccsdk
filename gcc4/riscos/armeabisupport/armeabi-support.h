/* armeabi-support.h
 *
 * Copyright 2019, 2020 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef ARMEABI_SUPPORT_H
#define ARMEABI_SUPPORT_H

#include <kernel.h>

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
#define ARMEABISUPPORT_MEMORYOP_ALLOCATOR_FROM_DA	11

#define ARMEABISUPPORT_STACKOP_ALLOC			0
#define ARMEABISUPPORT_STACKOP_FREE			1
#define ARMEABISUPPORT_STACKOP_GET_STACK		2
#define ARMEABISUPPORT_STACKOP_GET_STACK_BOUNDS		3
#define ARMEABISUPPORT_STACKOP_GET_STACK_SIZES		4

#define ARMEABISUPPORT_ALLOCATOR_TYPE_MASK		0xF
#define ARMEABISUPPORT_ALLOCATOR_TYPE_PAGE_MAPPED	1
#define ARMEABISUPPORT_ALLOCATOR_TYPE_HEAP		2
#define ARMEABISUPPORT_ALLOCATOR_TYPE_SINGLE_USE	3
#define ARMEABISUPPORT_ALLOCATOR_FLAG_GLOBAL		(1 << 5)

#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAG_ALLOC_ONLY	(0 << ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAG_CLAIM	(1 << ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAG_MAP		(2 << ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_MASK	(3 << ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT)
#define ARMEABISUPPORT_ALLOCATOR_ALLOC_FLAGS_SHIFT	16

#define XOS_Bit	0x020000
#define ARMEABISupport_MemoryOp        0x059D00
#define ARMEABISupport_AbortOp         0x059D01
#define ARMEABISupport_StackOp         0x059D02
#define ARMEABISupport_Cleanup         0x059D03

#define ARMEABI_PAGE_SIZE 0x1000
#define ARMEABI_PAGE_SHIFT 12

typedef void *armeabi_allocator_t;
typedef unsigned char *eabi_PTR;

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
		     eabi_PTR *block_ret)
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
		    eabi_PTR __block)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register eabi_PTR block asm("r2") = __block;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		: [err] "=r" (err)
		: "r" (allocator),
		  "r" (block),
		  "r" (size),
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
			   eabi_PTR __block,
			   unsigned __num_pages)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register eabi_PTR block asm("r2") = __block;
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
			     eabi_PTR __block,
			     unsigned __num_pages)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register eabi_PTR block asm("r2") = __block;
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
			 eabi_PTR __block,
			 unsigned __num_pages,
			 unsigned __flags)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register eabi_PTR block asm("r2") = __block;
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
			   eabi_PTR __block,
			   unsigned __num_pages)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register eabi_PTR block asm("r2") = __block;
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
armeabi_memory_info(armeabi_allocator_t allocator,
		    int *da_number_ret,
		    eabi_PTR *base_ret,
		    eabi_PTR *end_ret)
{
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	MOV	r1, %[allocator];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVS	%[err], r0;\n"
		"	BVS	0f;\n"
		"	TEQ	%[da_num], #0;\n"
		"	STRNE	r0, [%[da_num]];\n"
		"	TEQ	%[base],#0;\n"
		"	STRNE	r1, [%[base]];\n"
		"	TEQ	%[end],#0;\n"
		"	STRNE	r2, [%[end]];\n"
		"	MOV	%[err], #0;\n"
		"0:\n"
		: [err] "=r" (err)
		: [allocator] "r" (allocator),
		  [da_num] "r" (da_number_ret),
		  [base] "r" (base_ret),
		  [end] "r" (end_ret),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_ALLOCATOR_INFO)
		: "r0", "r1", "r2", "lr", "cc");
  return err;
}

static inline _kernel_oserror *
armeabi_memory_allocator_get_page_number(armeabi_allocator_t *__allocator,
					 eabi_PTR __addr,
					 int *page_number_ret)
{
  register armeabi_allocator_t allocator asm("r1") = __allocator;
  register eabi_PTR addr asm("r2") = __addr;
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
armeabi_memory_allocator_from_address(eabi_PTR __addr,
				      armeabi_allocator_t *allocator_ret)
{
  register eabi_PTR addr asm("r1") = __addr;
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
armeabi_memory_allocator_from_da(int __da,
				 armeabi_allocator_t *allocator_ret)
{
  register int da asm("r1") = __da;
  _kernel_oserror *err;

  asm volatile ("	MOV	r0, %[reason];\n"
		"	SWI	%[XARMEABISupport_MemoryOp];\n"
		"	MOVVC	%[err], #0;\n"
		"	MOVVS	%[err], r0;\n"
		"	MOVVC	%[allocator], r0;\n"
		"	MOVVS	%[allocator], #0;\n"
		: [err] "=r" (err), [allocator] "=r" (*allocator_ret)
		: "r" (da),
		  [XARMEABISupport_MemoryOp] "i" (XOS_Bit | ARMEABISupport_MemoryOp),
		  [reason] "I" (ARMEABISUPPORT_MEMORYOP_ALLOCATOR_FROM_DA)
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

static inline unsigned armeabi_bytes_to_pages(size_t size)
{
  return ((size + (ARMEABI_PAGE_SIZE - 1)) & ~(ARMEABI_PAGE_SIZE - 1)) >> ARMEABI_PAGE_SHIFT;
}

#endif
