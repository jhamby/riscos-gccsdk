@ setjmp, longjmp
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.weak	__alloca_list

	@ jmp_buf layout :
	@   +   0 ( 4) : contents __alloca_list
	@   +   4 ( 4) : __executing_signalhandler
	@   +   8 ( 4) : __pthread_worksemaphore
	@   +  12 (48) : f4, f5, f6, f7 on entry of setjmp()
	@   +  60 (24) : v1, v2, v3, v4, v5, v6 on entry of setjmp()
	@   +  84 ( 4) : sl
	@   +  88 ( 4) : fp
	@   +  92 ( 4) : sp
	@   +  96 ( 4) : lr
	@ = 100 bytes (= __JMP_BUF_SIZE * sizeof(int))

	.global	setjmp
	NAME	setjmp
setjmp:
 PICEQ "STMFD	sp!, {v4, lr}"
 PICEQ "BL	__rt_load_pic"

	@ record the current allocation pointer for use with longjmp
	@ Note, alloca_list is a weak symbol so may not be set
	LDR	a4, .L0	@=__pthread_running_thread
 PICEQ "LDR	a4, [v4, a4]"
	LDR	a4, [a4]
	LDR	a4, [a4, #__PTHREAD_ALLOCA_OFFSET + 8]
	STR	a4, [a1], #4

	LDR	a4, .L0+4	@=__executing_signalhandler
 PICEQ "LDR	a4, [v4, a4]"
	LDR	a4, [a4]
	STR	a4, [a1], #4

	LDR	a4, .L0+8	@=__pthread_worksemaphore
 PICEQ "LDR	a4, [v4, a4]"
	LDR	a4, [a4]
	STR	a4, [a1], #4

#ifndef __SOFTFP__
	SFM	f4, 4, [a1], #4*12
#else
	ADD	a1, a1, #4*12
#endif
 PICEQ "LDMFD	sp!, {v4, lr}"

	STMIA	a1, {v1, v2, v3, v4, v5, v6, sl, fp, sp, lr}

	MOV	a1, #0
	MOV	pc, lr
.L0:
	WORD	__pthread_running_thread
	WORD	__executing_signalhandler
	WORD	__pthread_worksemaphore
	DECLARE_FUNCTION setjmp

	.global	longjmp
	NAME	longjmp
longjmp:
	@ No need to save lr or PIC register as they are overwritten
	@ later on anyway. v4 is already used below, so use v6 as the PIC
	@ register.
 PICEQ "BL	__rt_load_pic"
 PICEQ "MOV	v6, v4"

	@ Free allocations that occurred after the setjmp. This must be
	@ done before any registers, including the fp registers, are
	@ restored from the jmp_buf, because free() could do anything.

	@ We should be able to safely use v1-v6, since if a recursive
	@ call to longjmp does occur, then the v1-v6 are going to be
	@ safely restored to their current values.

	LDR	v5, .L0	@=__pthread_running_thread
 PICEQ "LDR	v5, [v6, v5]"
	LDR	v5, [v5]
	ADD	v5, v5, #__PTHREAD_ALLOCA_OFFSET + 8
	LDR	v4, [a1], #4
	CMP	v5, #0
	LDRNE	v3, [v5]
	MOV	v1, a1
	MOV	v2, a2
	CMPNE	v3, v4
	BEQ	__longjmp_l3

__longjmp_l2:
	MOVS	a1, v3
	LDRNE	v3, [v3]		@ StrongARM order
	BEQ	__longjmp_fatal	@ oh fuck!, the list has run out
	@ update pointers in loop.
	@ must be done here in case free indirectly calls longjmp
	STR	v3, [v5]		@ update head of alloca_list
	STR	v3, [v1, #-4]		@ update the stored value in jmp_buf
	BL	free
	CMP	v3, v4
	BNE	__longjmp_l2

__longjmp_l3:
	@ Find the old stack pointer, and free any chunks
	LDR	sl, [v1, #84 - 4]
	LDR	sp, [v1, #92 - 4]
	BL	__trim_stack

	LDR	a1, .L0+4	@=__executing_signalhandler
 PICEQ "LDR	a1, [v6, a1]"
	LDR	lr, [v1], #4
	STR	lr, [a1]

	LDR	a1, .L0+8	@=__pthread_worksemaphore
 PICEQ "LDR	a1, [v6, a1]"
	LDR	lr, [v1], #4
	STR	lr, [a1]

#ifndef __SOFTFP__
	LFM	f4, 4, [v1], #4*12
#else
	ADD	v1, v1, #4*12
#endif

	MOVS	a1, v2
	MOVEQ	a1, #1			@ longjmp can't return 0
	@ Technically there is a problem here if we should be
	@ moving to a higher processor mode, such as USR -> SVC
	LDMIA	v1, {v1, v2, v3, v4, v5, v6, sl, fp, sp, pc}

	@ If we get here something has screwed up. The old value
	@ of alloca list which we were searching for which was active
	@ when the setjmp was called wasn't found. This can happen for
	@ at least two reasons.
	@  1. The jmpbuf has become corrupt through being overwritten
	@  2. The user is attempting a longjmp to an invalid setjmp
	@     point, i.e., the scope of the setjmp has exited.
__longjmp_fatal:
	ADR	a1, __longjmp_fatal_msg
	SWI	XOS_Write0
	@ This failure is too serious to trying raising a signal, since
	@ all the dynamic allocations have been freed and letting the user
	@ access that memory is just too dangerous. Further, the abort code
	@ or signal handler on abort may longjmp, causing recursion
	@ until the stack runs out. This is all likely to crash the users
	@ poor machine, so bale out now to be nice.
	MOV	a1, #EXIT_FAILURE
	B	_exit
__longjmp_fatal_msg:
	.byte	13, 10
	.byte	13, 10
	.ascii	"Fatal error detected in longjmp, jmpbuf possibly corrupt."
	.byte	13, 10
	.ascii	"This can happen for at least two reasons."
	.byte	13, 10
	.ascii	" - The jmpbuf has become corrupt through being overwritten."
	.byte	13, 10
	.ascii	" - The program is attempting a longjmp to an invalid setjmp point"
	.byte	13, 10
	.ascii	"   i.e. the scope of the setjmp has exited."
	.byte	13, 10, 0
	DECLARE_FUNCTION longjmp

	.end
