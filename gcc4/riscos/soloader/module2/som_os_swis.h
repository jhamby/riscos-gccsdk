/* som_os_swis.h
 *
 * Inline assembler versions of some common OS SWIs.
 *
 * Copyright 2007, 2011, 2012 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_OS_SWIS_H
#define SOM_OS_SWIS_H

#include <swis.h>
#include <stdlib.h>
#include <string.h>

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
  void *base_addr;
  int max_size;
} dynamic_area_block;

extern _kernel_oserror *dynamic_area_create (const char *name, int init_size,
					     int max_size,
					     dynamic_area_block *ret);

static inline _kernel_oserror *
dynamic_area_remove (unsigned int da_number)
{
  _kernel_oserror *err;

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

/* OS_Module SWIs.  */

enum
{
  reason_code_OSMODULE_CLAIM = 6,
  reason_code_OSMODULE_FREE
};

static inline _kernel_oserror *
RMA_claim (int size, void **block_ret)
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
RMA_free (void *block)
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

#endif
