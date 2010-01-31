/*
 * This file is intended to provide an interface between UnixLib and
 * programs and/or libraries initially developed using Norcroft headers
 * (in particular its kernel.h header).
 * Any simularity between this and the Norcroft kernel.h file is probably
 * due to having to use identical structs.
 *
 * Copyright (c) 2000-2009 UnixLib Developers
 */

#ifndef __KERNEL_H
#define __KERNEL_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

typedef struct
{
   int r[10];
} _kernel_swi_regs;

typedef struct
{
   int load, exec;
   int start, end;
} _kernel_osfile_block;

typedef struct
{
   int errnum;
   char errmess[252];
} _kernel_oserror;

typedef struct stack_chunk
{
  unsigned long sc_mark;
  struct stack_chunk *sc_next, *sc_prev;
  unsigned long sc_size;
  int (*sc_deallocate) (void);
} _kernel_stack_chunk;

extern int _kernel_fpavailable (void) __THROW;

/* Passes argument string to SWI "OS_CLI". */
extern int _kernel_oscli (const char *__s) __THROW __nonnull ((1));

#define _kernel_NONX 0x80000000
/* Call the SWI specified by 'no'. 'in' points to a register block
   for SWI entry. 'out' points to a register block for SWI exit.
   The X bit is set by _kernel_swi unless bit 31 is set.  */
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

/* Read the value of system variable 'name', placing the
   result in 'buffer'.  */
extern _kernel_oserror *
_kernel_getenv (const char *__name, char *__buffer, unsigned int __size)
     __THROW __nonnull ((1, 2)) __wur;

/* Set the system variable 'name' with 'value'. If 'value == 0' then
   'name' is deleted.  */
extern _kernel_oserror *
_kernel_setenv (const char *__name, const char *__value)
     __THROW __nonnull ((1, 2)) __wur;

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

/* Signed divide and remainder function. Returns the remainder in R1. */
extern int _kernel_sdiv (int __divisor, int __dividend)
     __THROW __attribute__ ((__const__)) __wur;

/* Signed remainder function.  */
extern int _kernel_srem (int __divisor, int __dividend)
     __THROW __attribute__ ((__const__)) __wur;

/* Signed divide and remainder function by 10.
   Returns the remainder in R1. */
extern int _kernel_sdiv10 (int __dividend)
     __THROW __attribute__ ((__const__)) __wur;

/* Return a pointer to the current stack chunk.  */
extern _kernel_stack_chunk *_kernel_current_stack_chunk (void) __THROW;

__END_DECLS

#endif
