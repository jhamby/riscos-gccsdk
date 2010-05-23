/*
 * This file is intended to provide an interface between UnixLib and
 * programs and/or libraries initially developed using Norcroft headers
 * (in particular its kernel.h header).
 * Any simularity between this and the Norcroft kernel.h file is probably
 * due to having to use identical structs.
 *
 * Copyright (c) 1997-2005 Nick Burrett
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#ifndef __KERNEL_H
#define __KERNEL_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifdef __TARGET_SCL__
#  define __need_size_t
#  include <stddef.h>
#endif

__BEGIN_DECLS

typedef struct
{
   int errnum;
   char errmess[252];
} _kernel_oserror;

/* Return 0 when no floating point emulator nor hardware is available.  */
extern int _kernel_fpavailable (void) __THROW;

typedef struct
{
   int r[10];
} _kernel_swi_regs;

#define _kernel_NONX 0x80000000
/* Call the SWI specified by 'no'. 'in' points to a register block
   for SWI entry. 'out' points to a register block for SWI exit.
   The X bit is set by _kernel_swi unless bit 31 (_kernel_NONX) is set
   (in this case, when an error happens, _kernel_swi does not return).  */
extern _kernel_oserror *_kernel_swi (int __no,
				     const _kernel_swi_regs *__in,
				     _kernel_swi_regs *__out) __THROW;

/* Similar to _kernel_swi but the carry flag status is returned in
   'carry'.  */
extern _kernel_oserror *_kernel_swi_c (int __no,
				       const _kernel_swi_regs *__in,
				       _kernel_swi_regs *__out,
				       int *__carry)
     __THROW;

/* Return the last OS error since the last time _kernel_last_oserror
   was called.  Return zero if no errors have occurred.  */
extern _kernel_oserror *_kernel_last_oserror (void) __THROW __wur;

/* Following int returning calls:
     When >= 0, the call succeeded.
     When -1, the call failed but there is no OS error (like pressing Escape
       during _kernel_osrdch), related to C flag set.
     When -2, the call failed and the OS error can be queried using
       _kernel_last_oserror.  */
#define _kernel_ERROR (-2)

/* Perform an OS_Byte operation.
   R1 is returned in the bottom byte, R2 in the second byte,
   if carry set, then third byte = 1.  */
extern int _kernel_osbyte (int __operation, int __x, int __y) __THROW;

/* Read a character from the OS input stream.  */
extern int _kernel_osrdch (void) __THROW;

/* Write a character to the OS output streams.  The return value indicates
   success or failure.  */
extern int _kernel_oswrch (int __ch) __THROW;

/* Return the next byte from the file 'handle'. Return -1 on EOF.  */
extern int _kernel_osbget (unsigned int __handle) __THROW;

/* Write the byte 'ch' to the file 'handle'. Return success or failure.  */
extern int _kernel_osbput (int __ch, unsigned int __handle) __THROW;

typedef struct
{
   int load, exec;
   int start, end;
} _kernel_osfile_block;

/* Perform an OS_File operation. The _kernel_osfile_block provides
   values for registers R2-R5.  */
extern int _kernel_osfile (int __operation, const char *__name,
			   _kernel_osfile_block *__inout)
     __THROW __nonnull ((2, 3));

typedef struct
{
  void * dataptr;
  int nbytes, fileptr;
  int buf_len;
  char *wild_fld;
} _kernel_osgbpb_block;

/* Read/write a number of bytes on file 'handle'. */
extern int _kernel_osgbpb (int __operation, unsigned int __handle,
			   _kernel_osgbpb_block *__inout)
     __THROW __nonnull ((3));

/* Perform an OS_Word operation.  */
extern int _kernel_osword (int __operation, int *__data)
     __THROW __nonnull ((2));

/* Open or close a file. Open returns a file handle, close just
   indicates success/failure.  */
extern int _kernel_osfind (int __operation, const char *__name)
     __THROW __nonnull ((2));

/* Perform an OS_Args operation. Generally returns the value in R2,
   unless op = 0.  */
extern int _kernel_osargs (int __operation, unsigned int __handle,
			   int __arg) __THROW;

/* Call OS_CLI with the string 's'. Not to be used to invoke other
   applications (use _kernel_system otherwise) as abort handlers are
   left installed).  */
extern int _kernel_oscli (const char *__s) __THROW __nonnull ((1));

/* Read the value of system variable 'name', placing the
   result in 'buffer'.  */
extern _kernel_oserror *_kernel_getenv (const char *__name, char *__buffer,
                                        unsigned int __size)
     __THROW __nonnull ((1, 2)) __wur;

/* Set the system variable 'name' with 'value'. If 'value == NULL' then
   'name' is deleted.  */
extern _kernel_oserror *_kernel_setenv (const char *__name,
                                        const char *__value)
     __THROW __nonnull ((1, 2)) __wur;

/* Language support.  */

/* Unsigned divide and remainder function. Returns the remainder in R1. */
extern unsigned int _kernel_udiv (unsigned int __divisor,
				  unsigned int __dividend)
     __THROW __attribute__ ((__const__)) __wur;

/* Unsigned remainder function.  */
extern unsigned int _kernel_urem (unsigned int __divisor,
				  unsigned int __dividend)
     __THROW __attribute__ ((__const__)) __wur;

/* Unsigned divide and remainder function by 10.
   Returns the remainder in R1. */
extern unsigned int _kernel_udiv10 (unsigned int __dividend)
     __THROW __attribute__ ((__const__)) __wur;

/* Signed divide and remainder function (rounding towards zero). Returns the
   remainder in R1. */
extern int _kernel_sdiv (int __divisor, int __dividend)
     __THROW __attribute__ ((__const__)) __wur;

/* Signed remainder function (rounding towards zero).  */
extern int _kernel_srem (int __divisor, int __dividend)
     __THROW __attribute__ ((__const__)) __wur;

/* Signed divide and remainder function by 10 (rounding towards zero).
   Returns the remainder in R1. */
extern int _kernel_sdiv10 (int __dividend)
     __THROW __attribute__ ((__const__)) __wur;

#ifdef __TARGET_SCL__
/* Allocates a block of memory of the size specified for a Variable Length
   Array. The current implementation will not be automatically freed on return
   from the calling function. Clients using longjmp should take care to retain
   their own lists of allocated storage as it is not guaranteed to be tracked
   by the implementation. Returns a pointer to the allocated space.  */
extern void *__rt_allocauto (size_t size);

/* Frees a block of memory allocated by the __rt_allocauto function. This call
   will be automatically added by the compiler as automatically allocated
   storage goes out of scope.  */
extern void __rt_freeauto (void *);
#endif

typedef struct stack_chunk
{
  unsigned long sc_mark; /* == 0xf60690ff */
  struct stack_chunk *sc_next, *sc_prev;
  unsigned long sc_size;
  int (*sc_deallocate) (void);
} _kernel_stack_chunk;

/* Return a pointer to the current stack chunk.  */
extern _kernel_stack_chunk *_kernel_current_stack_chunk (void) __THROW;

#ifdef __TARGET_SCL__
typedef union
{
  struct
   {
     int s:1, u:16, x: 15;
     unsigned mhi, mlo;
   } i;
  int w[3];
} _extended_fp_number;

typedef struct
{
  int r4, r5, r6, r7, r8, r9;
  int fp, sp, pc, sl;
  _extended_fp_number f4, f5, f6, f7;
} _kernel_unwindblock;

/* Unwind the call stack one level. Return: >0 on success,
   0 if stack end has been reached, <0 any other failures.
   Input _kernel_unwindblock's fp, sl and pc values must be correct.
   r4-r9 and f4-f7 are updated if the frame addressed by the input value
   of fp contains saved values for the corresponding registers.
   fp, sp, sl and pc are always updated.
   *language is returned a pointer to a string naming the language
   corresponding to the returned value of pc.  */
extern int _kernel_unwind (_kernel_unwindblock *__inout, char **__language);

/* Program environment functions.  */

/* Return a pointer to the name of the function that contains the
   address 'pc' (or NULL if no name can be found).  */
extern char *_kernel_procname (int __pc);

/* Return a pointer to the name of the language that the address
   'pc' lies within.  NULL if the language is unknown. */
extern char *_kernel_language (int __pc);

/* Return a pointer to the command string used to run the program.  */
extern char *_kernel_command_string (void);

/* Set the return code to be used by _kernel_exit.  */
extern void _kernel_setreturncode (unsigned int __code);

/* Call OS_Exit. Use the return code set by _kernel_setreturncode.  */
extern void _kernel_exit (int __value) __attribute__ ((__noreturn__));

/* Generates an external error and return to parent.  When error pointer
   is NULL, just return to parent.  */
extern void _kernel_raise_error (_kernel_oserror *);

/* Reset the InTrapHandler flag to prevent recursive traps.  */
extern void _kernel_exittraphandler (void);

/* Returns 6 for RISC OS.  */
extern int _kernel_hostos (void);

/* Return 1 if there has been an Escape since the previous call to
   _kernel_escape_seen.  */
extern int _kernel_escape_seen (void);

extern int _kernel_processor_mode(void);

/* Enable IRQ interrupts.  This can only be executed within SVC mode.  */
extern void _kernel_irqs_on (void);

/* Disable IRQ interrupts.  This can only be executed within SVC mode.  */
extern void _kernel_irqs_off (void);

/* Return non-zero if IRQs are disabled.  */
extern int _kernel_irqs_disabled (void);

/* Call OS_CLI with the string 's'. If chain == 0, then the current
   application is copied away and the new application executed as
   a subprogram. Control will return to the main application once the
   subprogram has completed.
   If chain == 1, then _kernel_system will not return.  */
extern int _kernel_system (const char *__string, int __chain);

/* Memory allocation functions.  */

/* Tries to allocate a block of at least 'words' words. If it fails,
   it allocates the largest possible block (may be size 0).
   *block is returned a pointer to the start of the allocated block
   (NULL if 'a block of size zero' has been allocated).  */
extern unsigned _kernel_alloc (unsigned int __words, void **__block);

/* Register procedures to be used by the SharedCLibrary when it
   creates to frees memory e.g. on creation of stack chunks.
   Those procedures may not check for stack overflow and must not
   use more than 41 words of stack.  */
typedef void freeproc (void *);
typedef void *allocproc (unsigned);

extern void _kernel_register_allocs (allocproc *__malloc, freeproc *__free);

/* If the heap limit != application limit, then the procedure
   registered here is used to request n bytes from the memory
   allocation routine.  */
typedef int _kernel_ExtendProc (int __n, void **__p);

_kernel_ExtendProc *_kernel_register_slotextend (_kernel_ExtendProc *__proc);

extern void *_kernel_RMAalloc (size_t size);

extern void *_kernel_RMAextend (void *p, size_t size);

extern void _kernel_RMAfree (void *p);
#endif

__END_DECLS

#endif
