/* som_os_swis.h
 *
 * Inline assembler versions of some common OS SWIs.
 *
 * Copyright 2007 GCCSDK Developers
 */

#ifndef SOM_OS_SWIS_H
#define SOM_OS_SWIS_H

#ifndef __SWIS_H
#include <swis.h>
#endif

#ifndef __STDLIB_H
#include <stdlib.h>
#endif

/* Dynamic Area SWIs */
enum {
  reason_code_DYNAMIC_AREA_CREATE,
  reason_code_DYNAMIC_AREA_REMOVE,
  reason_code_DYNAMIC_AREA_INFO,
  reason_code_DYNAMIC_AREA_ENUM,
  reason_code_DYNAMIC_AREA_RENUMBER,
  reason_code_DYNAMIC_AREA_FREE_SPACE
};

typedef struct dynamic_area_block
{
  unsigned int	number;
  void *	base_addr;
  int		max_size;

} dynamic_area_block;

_kernel_oserror *dynamic_area_create(const char *name, int init_size, int max_size, dynamic_area_block *ret);

static inline _kernel_oserror *dynamic_area_remove(unsigned int da_number)
{
_kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[da_number];\n\t"
		"SWI	%[os_dynamic_area];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [da_number] "r" (da_number),
		  [os_dynamic_area] "i" (XOS_Bit | OS_DynamicArea), [reason] "I" (reason_code_DYNAMIC_AREA_REMOVE)
		: "r0", "r1", "r14", "cc");
  return err;
}

static _kernel_oserror *dynamic_area_extend(unsigned int da_number, int by)
{
_kernel_oserror *err;

  asm volatile ("MOV	r0, %[da_number];\n\t"
		"MOV	r1, %[by];\n\t"
		"SWI	%[os_change_dynamic_area];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [da_number] "r" (da_number), [by] "r" (by), [os_change_dynamic_area] "i" (XOS_Bit | OS_ChangeDynamicArea)
		: "r0", "r1", "lr", "cc");
  return err;
}

/* Heap SWIs */
enum {
  reason_code_HEAP_INIT,
  reason_code_HEAP_DESCRIBE,
  reason_code_HEAP_CLAIM,
  reason_code_HEAP_RELEASE,
  reason_code_HEAP_CHANGE_BLOCK_SIZE,
  reason_code_HEAP_CHANGE_HEAP_SIZE,
  reason_code_HEAP_READ_BLOCK_SIZE
};

static inline _kernel_oserror *heap_init(void *base_addr, int size)
{
_kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[base_addr];\n\t"
		"MOV	r3, %[size];\n\t"
		"SWI	%[os_heap];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [base_addr] "r" (base_addr), [size] "rI" (size),
		  [os_heap] "i" (XOS_Bit | OS_Heap), [reason] "I" (reason_code_HEAP_INIT)
		: "r0", "r1", "r2", "r14", "cc");
  return err;
}

static inline _kernel_oserror *heap_claim(void *base_addr, int size, void **block_ret)
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
		: [base_addr] "r" (base_addr), [size] "rI" (size),
		  [os_heap] "i" (XOS_Bit | OS_Heap), [reason] "I" (reason_code_HEAP_CLAIM)
		: "r0", "r1", "r2", "r3", "r14", "cc");

  if (block_ret)
    *block_ret = block;

  return err;
}

static inline _kernel_oserror *heap_release(void *base_addr, void *block)
{
_kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[base_addr];\n\t"
		"MOV	r2, %[block];\n\t"
		"SWI	%[os_heap];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [base_addr] "r" (base_addr), [block] "r" (block),
		  [os_heap] "i" (XOS_Bit | OS_Heap), [reason] "I" (reason_code_HEAP_RELEASE)
		: "r0", "r1", "r2", "r14", "cc");
  return err;
}

static inline _kernel_oserror *heap_extend(void *base_addr, int by)
{
_kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r1, %[base_addr];\n\t"
		"MOV	r3, %[by];\n\t"
		"SWI	%[os_heap];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [base_addr] "r" (base_addr), [by] "rI" (by),
		  [os_heap] "i" (XOS_Bit | OS_Heap), [reason] "I" (reason_code_HEAP_CHANGE_HEAP_SIZE)
		: "r0", "r1", "r2", "r3", "r14", "cc");
  return err;
}

/* OS_Module SWIs */

enum
{
  reason_code_OSMODULE_CLAIM = 6,
  reason_code_OSMODULE_FREE
};

static inline _kernel_oserror *RMA_claim(int size, void **block_ret)
{
_kernel_oserror *err;
void *block;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r3, %[size];\n\t"
		"SWI	%[os_module];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		"MOVVS	%[block], #0;\n\t"
		"MOVVC	%[block], r2;\n\t"
		: [err] "=r" (err), [block] "=r" (block)
		: [size] "r" (size), [os_module] "i" (XOS_Bit | OS_Module), [reason] "I" (reason_code_OSMODULE_CLAIM)
		: "r0", "r1", "r2", "r3", "lr", "cc");

  if (block_ret)
    *block_ret = block;

  return err;
}

static inline _kernel_oserror *RMA_free(void *block)
{
_kernel_oserror *err;

  asm volatile ("MOV	r0, %[reason];\n\t"
		"MOV	r2, %[block];\n\t"
		"SWI	%[os_module];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [block] "r" (block), [os_module] "i" (XOS_Bit | OS_Module), [reason] "I" (reason_code_OSMODULE_FREE)
		: "r0", "r1", "r2", "lr", "cc");
  return err;
}

/* General SWIs */

static inline unsigned int os_read_monotonic_time(void)
{
unsigned int t;

  asm volatile ("SWI	%[os_read_monotonic_time];\n\t"
		"MOV	%[t], r0;\n\t"
		: [t] "=r" (t)
		: [os_read_monotonic_time] "i" (XOS_Bit | OS_ReadMonotonicTime)
		: "r0", "lr", "cc");
  return t;
}

static inline _kernel_oserror *os_call_every(unsigned int t, void (*handler)(void), void *pw)
{
_kernel_oserror *err;

  asm volatile ("MOV	r0, %[time];\n\t"
		"MOV	r1, %[handler];\n\t"
		"MOV	r2, %[pw];\n\t"
		"SWI	%[os_callevery];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [time] "r" (t), [handler] "r" (handler), [pw] "r" (pw), [os_callevery] "i" (XOS_Bit | OS_CallEvery)
		: "r0", "r1", "r2", "r14", "cc");
  return err;
}

static inline _kernel_oserror *os_remove_ticker_event(void (*handler)(void), void *pw)
{
_kernel_oserror *err;

  asm volatile ("MOV	r0, %[handler];\n\t"
		"MOV	r1, %[pw];\n\t"
		"SWI	%[os_removetickerevent];\n\t"
		"MOVVS	%[err], r0;\n\t"
		"MOVVC	%[err], #0;\n\t"
		: [err] "=r" (err)
		: [handler] "r" (handler), [pw] "r" (pw), [os_removetickerevent] "i" (XOS_Bit | OS_RemoveTickerEvent)
		: "r0", "r1", "r14", "cc");
  return err;
}

static inline void os_add_callback(void (*handler)(void), void *pw)
{
  asm volatile ("MOV	r0, %[handler];\n\t"
		"MOV	r1, %[pw];\n\t"
		"SWI	%[os_add_callback];\n\t"
		:
		: [handler] "r" (handler), [pw] "r" (pw), [os_add_callback] "i" (XOS_Bit | OS_AddCallBack)
		: "r0", "r1", "r14", "cc");
}

static inline void os_remove_callback(void (*handler)(void), void *pw)
{
  asm volatile ("MOV	r0, %[handler];\n\t"
		"MOV	r1, %[pw];\n\t"
		"SWI	%[os_remove_callback];\n\t"
		:
		: [handler] "r" (handler), [pw] "r" (pw), [os_remove_callback] "i" (XOS_Bit | OS_RemoveCallBack)
		: "r0", "r1", "r14", "cc");
}

/* s updated to point to terminating character. */
static inline unsigned int os_read_unsigned(const char **s, int base)
{
unsigned int res;

  asm volatile ("MOV	r0, %[base];\n\t"
		"LDR	r1, [%[s], #0];\n\t"
		"SWI	%[os_read_unsigned];\n\t"
		"STR	r1, [%[s], #0];\n\t"
		"MOV	%[res], r2;\n\t"
		: [res] "=r" (res)
		: [base] "r" (base), [s] "r" (s), [os_read_unsigned] "i" (XOS_Bit | OS_ReadUnsigned)
		: "r0", "r1", "r2", "lr", "cc", "memory");
  return res;
}

#endif
