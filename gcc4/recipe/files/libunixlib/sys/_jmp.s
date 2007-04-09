@ setjmp, longjmp
@ Copyright (c) 2000-2007 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	@ jmp_buf layout (hard-float) :
	@   +   0 ( 4) : __pthread_running_thread->alloca
	@   +   4 ( 4) : __executing_signalhandler
	@   +   8 ( 4) : __pthread_worksemaphore
	@   +  12 (48) : f4, f5, f6, f7 on entry of setjmp()
	@   +  60 (24) : v1, v2, v3, v4, v5, v6 on entry of setjmp()
	@   +  84 ( 4) : sl
	@   +  88 ( 4) : fp
	@   +  92 ( 4) : sp
	@   +  96 ( 4) : lr
	@ = 100 bytes (= __JMP_BUF_SIZE * sizeof(int))

	@ jmp_buf layout (soft-float) :
	@   +   0 ( 4) : __pthread_running_thread->alloca
	@   +   4 ( 4) : __executing_signalhandler
	@   +   8 ( 4) : __pthread_worksemaphore
	@   +  12 (24) : v1, v2, v3, v4, v5, v6 on entry of setjmp()
	@   +  36 ( 4) : sl
	@   +  40 ( 4) : fp
	@   +  44 ( 4) : sp
	@   +  48 ( 4) : lr
	@ = 52 bytes (= __JMP_BUF_SIZE * sizeof(int))

	.global	setjmp
	NAME	setjmp
setjmp:
 PICEQ "STMFD	sp!, {v4, lr}"
 PICEQ "BL	__rt_load_pic"

	LDR	a4, .L0 + 0	@ =__pthread_running_thread
 PICEQ "LDR	a4, [v4, a4]"
	LDR	a4, [a4]
	LDR	a4, [a4, #__PTHREAD_ALLOCA_OFFSET]
	STR	a4, [a1], #4

	LDR	a4, .L0 + 4	@ =__executing_signalhandler
 PICEQ "LDR	a4, [v4, a4]"
	LDR	a4, [a4]
	STR	a4, [a1], #4

	LDR	a4, .L0 + 8	@ =__pthread_worksemaphore
 PICEQ "LDR	a4, [v4, a4]"
	LDR	a4, [a4]
	STR	a4, [a1], #4

#ifndef __SOFTFP__
	SFM	f4, 4, [a1], #4*12
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
	@ later on anyway.
 PICEQ "BL	__rt_load_pic"	@ Sets up v4

	@ We should be able to safely use v1-v6, since if a recursive
	@ call to longjmp does occur, then the v1-v6 are going to be
	@ safely restored to their current values.

	MOV	v1, a1
	MOV	v2, a2

	@ Restore the stack status at setjmp() time, and free any chunks
#ifndef __SOFTFP__
	ADD	sl, v1, #84
#else
	ADD	sl, v1, #36
#endif
	LDMIA	sl, {sl, fp, sp}
	BL	__trim_stack	@ Destroys a1 - a4, ip, lr

	@ Make sure we free any alloca() memory blocks between the point
	@ of setjmp() and longjmp().
	LDR	a1, .L0 + 0	@ =__pthread_running_thread
 PICEQ "LDR	a1, [v4, a1]"
	LDR	a1, [a1]
	LDR	a2, [a1, #__PTHREAD_ALLOCA_OFFSET]
	LDR	a1, [v1], #4	@ a1 = new __pthread_running_thread->alloca
	TEQ	a1, a2
	BLNE	__gcc_alloca_longjmp_free

	LDR	a1, .L0 + 4	@ =__executing_signalhandler
 PICEQ "LDR	a1, [v4, a1]"
	LDR	lr, [v1], #4
	STR	lr, [a1]

	LDR	a1, .L0 + 8	@ =__pthread_worksemaphore
 PICEQ "LDR	a1, [v4, a1]"
	LDR	lr, [v1], #4
	STR	lr, [a1]

#ifndef __SOFTFP__
	LFM	f4, 4, [v1], #4*12
#endif

	MOVS	a1, v2
	MOVEQ	a1, #1			@ longjmp can't return 0
	@ Technically there is a problem here if we should be
	@ moving to a higher processor mode, such as USR -> SVC
	LDMIA	v1, {v1, v2, v3, v4, v5, v6, sl, fp, sp, pc}
	DECLARE_FUNCTION longjmp

	.end
