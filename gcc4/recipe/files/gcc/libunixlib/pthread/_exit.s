@ Call alloca thread free functions as necessary
@ Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.weak	__gcc_alloca_thread_free_all

	.global	__pthread_exit

	NAME	__pthread_exit
__pthread_exit:
	PIC_LOAD a2

	LDR	a1, funcs
 PICEQ "LDR	a1, [a2, a1]"
	TEQ	a1, #0
	MOVNE	pc, a1
	MOV	pc, r14

funcs:
	WORD	__gcc_alloca_thread_free_all
	DECLARE_FUNCTION __pthread_exit

	.end
