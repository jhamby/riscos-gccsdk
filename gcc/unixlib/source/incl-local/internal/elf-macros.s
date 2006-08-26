@ UnixLib assembler helper macros (ELF version)
@ Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers
@
@ This file and aof-macros.s must be kept in sync with each other

#ifndef __SOFTFP__
#define __SOFTFP__ 0
#endif

	@ Turn on macro extensions, allowing the use of LOCAL.
	.altmacro

	@ Macro for embedding function names in code, just before
	@ the function prologue.
	.macro	NAME	name
	LOCAL t0, t1
t0:
	.asciz	"\name"
	.align
t1:
	.word	0xff000000 + (t1 - t0)
	.endm

	@ Assembler equivalent of __set_errno in errno.h.
	@ #define __set_errno(val) (errno = (val), -1)
	@ Entry condition
	@   val = new error code
	@ Exit condition
	@   val = -1
	@   Rerrno destroyed

	.macro	__set_errno	val, Rerrno
	.if __UNIXLIB_ERRNO_THREADED > 0
	ldr	\Rerrno, =__pthread_running_thread
	ldr	\Rerrno, [\Rerrno]
	str	\val, [\Rerrno, #__PTHREAD_ERRNO_OFFSET]
	.else
	ldr	\Rerrno, =errno
	str	\val, [\Rerrno]
	.endif
	mov	\val, #-1
	.endm


	.macro	__get_errno	val, Rerrno
	.if __UNIXLIB_ERRNO_THREADED > 0
	ldr	\Rerrno, =__pthread_running_thread
	ldr	\Rerrno, [\Rerrno]
	ldr	\val, [\Rerrno, #__PTHREAD_ERRNO_OFFSET]
	.else
	ldr	\Rerrno, =errno
	ldr	\val, [\Rerrno]
	.endif
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
	movvc	a1, #-
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
