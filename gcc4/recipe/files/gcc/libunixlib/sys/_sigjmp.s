@ sigsetjmp, siglongjmp
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text


	@ sigsetjmp (sigjmp_buf state, int value)
	.global	sigsetjmp
	NAME	sigsetjmp
sigsetjmp:
	@ It's best to preserve these registers.
	STMFD	sp!, {a1, lr}
	BL	__sigsetjmp_helper
	LDMFD	sp!, {a1, lr}
	B	setjmp
	DECLARE_FUNCTION sigsetjmp

	@ siglongjmp (sigjmp_buf state, int value)
	.global	siglongjmp
	NAME	siglongjmp
siglongjmp:
	@ Preserve state and value
	@ __siglongjmp_helper will update the process's blocked signal
	@ mask, if it was saved by sigsetjmp. If there are any pending
	@ signals unblocked, then these will be delivered.
	STMFD	sp!, {a1, a2}
	BL	__siglongjmp_helper
	LDMFD	sp!, {a1, a2}
	B	longjmp
	DECLARE_FUNCTION siglongjmp

	.end
