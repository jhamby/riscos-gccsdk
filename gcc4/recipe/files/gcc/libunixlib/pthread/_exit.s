@ Call alloca thread free functions as necessary
@ Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.weak	__gcc_alloca_thread_free_all

	.global	__pthread_exit

	@ For the shared library, we assume that __pthread_exit is only ever
	@ called by pthread_exit() and that v4 already contains the GOT
	@ pointer.
	NAME	__pthread_exit
__pthread_exit:
	LDR	a1, funcs
 PICEQ "LDR	a1, [v4, a1]"
	TEQ	a1, #0
	MOVNE	pc, a1
	MOV	pc, r14

funcs:
	WORD	__gcc_alloca_thread_free_all
	DECLARE_FUNCTION __pthread_exit

	.end
