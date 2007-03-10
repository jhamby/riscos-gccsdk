@ Call alloca thread free functions as necessary
@ Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.weak	__alloca_thread_free_all
	.weak	___arm_alloca_thread_free_all
	.weak	__gcc_alloca_thread_free_all

	.global	__pthread_exit

	@ For the shared library, we assume that __pthread_exit is only ever
	@ called by pthread_exit() and that v4 already contains the GOT pointer.
	NAME	__pthread_exit
__pthread_exit:
	STMFD	sp!, {v1,lr}
	ADR	v1, funcs

#ifndef __ELF__
	LDR	a1, [v1], #4	@funcs + 0
 PICEQ "LDR	a1, [v4, a1]"
	TEQ	a1, #0
	MOVNE	lr, pc
	MOVNE	pc, a1

	LDR	a1, [v1], #4	@funcs + 4
 PICEQ "LDR	a1, [v4, a1]"
	TEQ	a1, #0
	MOVNE	lr, pc
	MOVNE	pc, a1
#endif

	LDR	a1, [v1]	@funcs + 8
 PICEQ "LDR	a1, [v4, a1]"
	TEQ	a1, #0
	MOVNE	lr, pc
	MOVNE	pc, a1

	LDMFD	sp!, {v1,pc}

funcs:
#ifndef __ELF__
	WORD	__alloca_thread_free_all	@ Not needed in ELF builds
	WORD	___arm_alloca_thread_free_all	@ Not needed in ELF builds
#endif
	WORD	__gcc_alloca_thread_free_all
	DECLARE_FUNCTION __pthread_exit

	.end
