@ UnixLib assembler helper macros
@ Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007 UnixLib Developers

	@ Turn on macro extensions, allowing the use of LOCAL.
@	.altmacro

	@ Macro for embedding function names in code, just before
	@ the function prologue.
	.macro	NAME	name
@	LOCAL t0, t1
0:
	.asciz	"\name"
	.align
1:
	.word	0xff000000 + (1b - 0b)
	.endm

	@ Assembler equivalent of __set_errno in errno.h.
	@ #define __set_errno(val) (errno = (val), -1)
	@ Entry condition
	@   val = new error code
	@   v4 must be valid as the PIC register when compiling
	@   for the shared library
	@ Exit condition
	@   val = -1
	@   Rerrno destroyed

	.macro	__set_errno	val, Rerrno
#if __UNIXLIB_ERRNO_THREADED
	ldr	\Rerrno, 0f	@=__pthread_running_thread
 PICEQ "ldr	\Rerrno, [v4, \Rerrno]"
	ldr	\Rerrno, [\Rerrno]
	str	\val, [\Rerrno, #__PTHREAD_ERRNO_OFFSET]
	B	1f
0:
	WORD	__pthread_running_thread
#else
	ldr	\Rerrno, 0f	@=errno
 PICEQ "ldr	\Rerrno, [v4, \Rerrno]"
	str	\val, [\Rerrno]
	B	1f
0:
	WORD	errno
#endif
1:
	mov	\val, #-1
	.endm


	@ if compiling for shared library, v4 must be valid as the PIC register
	.macro	__get_errno	val, Rerrno
#if __UNIXLIB_ERRNO_THREADED
	ldr	\Rerrno, 0f	@=__pthread_running_thread
 PICEQ "ldr	\Rerrno, [v4, \Rerrno]"
	ldr	\Rerrno, [\Rerrno]
	ldr	\val, [\Rerrno, #__PTHREAD_ERRNO_OFFSET]
	B	1f
0:
	WORD	__pthread_running_thread
#else
	ldr	\Rerrno, 0f	@=errno
 PICEQ "ldr	\Rerrno, [v4, \Rerrno]"
	ldr	\val, [\Rerrno]
	B	1f
0:
	WORD	errno
#endif
1:
	.endm

	@ NetSWI, NetSWIsimple, NetSWI0 and NetSWIsimple0 are macros to call
	@ one of the networking (TCP/IP) SWIs.

	@ Calls networking (TCP/IP) SWI.
	.macro	NetSWI	swiname
	swi	\swiname
	blvs	__net_error
	.endm


	@ Calls networking (TCP/IP) SWI and returns to lr.
	.macro	NetSWIsimple	swiname
	swi	\swiname
	movvc	pc, lr
	b	__net_error
	.endm

	@ Calls networking (TCP/IP) SWI.
	@ Returns 0 in R0 for success, a non zero errno in R0 when
	@ error happened.
	.macro	NetSWI0	swiname
	swi	\swiname
	movvc	a1, #0
	blvs	__net_error
	.endm


	@ Calls networking (TCP/IP) SWI and returns to lr.
	@ Returns 0 in R0 for success, a non zero errno in R0 when
	@ error happened.
	.macro	NetSWIsimple0	swiname
	swi	\swiname
	movvc	a1, #0
	movvc	pc, lr
	b	__net_error
	.endm


	@ Macro to change processor modes and interrupt flags
	@ Works in 26bit or 32bit modes, on all architectures
	@ Use e.g. CHGMODE a1, SVC_Mode+IFlag
	.macro	CHGMODE	scratch, mode
	teq	a1, a1	@ Set Z
	teq	pc, pc	@ EQ if 32-bit mode
	teqnep	pc, #\mode
	mrseq	\scratch, CPSR	@ Acts a NOP for TEQP
	biceq	\scratch, \scratch, #0xcf	@ Preserve 32bit mode bit
	.if	\mode <> 0
	orreq	\scratch, \scratch, #((\mode) && 0xf) + ((\mode) >> 20)
	.endif
	msreq	CPSR_c, \scratch
	mov	a1, a1	@ Avoid StrongARM MSR bug
	.endm

	@ Macro for declaring the type and size of a function defined in ARM code.
	@ Place at end of function definition for size to be calculated correctly.
	.macro	DECLARE_FUNCTION name
	.type	\name, %function
	.size	\name, . - \name
	.endm

	@ Macro for declaring the type and size of an object defined in ARM code.
	@ Place at end of object definition for size to be calculated correctly.
	.macro	DECLARE_OBJECT name
	.type	\name, %object
	.size	\name, . - \name
	.endm

	@ Macro to define a word of data either with a GOT relocation for
	@ the shared library or with no relocation for the static library.
	.macro	WORD name
#ifdef PIC
	.word	\name(GOT)
#else
	.word	\name
#endif
	.endm

	@ Macro to conditionally assemble instruction for use in the shared library.
	.macro PICEQ instr
#ifdef PIC
	\instr
#endif
	.endm

	@ Macro to conditionally assemble instruction for use in the static library.
	.macro PICNE instr
#ifndef PIC
	\instr
#endif
	.endm
