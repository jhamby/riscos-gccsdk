; Call alloca thread free functions as necessary
; Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__alloca_thread_free_all|, WEAK
	IMPORT	|___arm_alloca_thread_free_all|, WEAK
	IMPORT	|__gcc_alloca_thread_free_all|, WEAK

	EXPORT	|__pthread_exit|

	; For the shared library, we assume that __pthread_exit is only ever
	; called by pthread_exit() and that v4 already contains the GOT pointer.
	NAME	__pthread_exit
|__pthread_exit|
	STMFD	sp!, {lr}
	ADR	a2, funcs

	[ __UNIXLIB_ELF = 0
	LDR	a1, [a2], #4	;funcs + 0
 PICEQ "LDR	a1, [v4, a1]"
	TEQ	a1, #0
	MOVNE	lr, pc
	MOVNE	pc, a1

	LDR	a1, [a2], #4	;funcs + 4
 PICEQ "LDR	a1, [v4, a1]"
	TEQ	a1, #0
	MOVNE	lr, pc
	MOVNE	pc, a1
	]

	LDR	a1, [a2]	;funcs + 8
 PICEQ "LDR	a1, [v4, a1]"
	TEQ	a1, #0
	MOVNE	lr, pc
	MOVNE	pc, a1

	LDMFD	sp!, {pc}

|funcs|
	[ __UNIXLIB_ELF = 0
	WORD	|__alloca_thread_free_all|	; Not needed in ELF builds
	WORD	|___arm_alloca_thread_free_all|	; Not needed in ELF builds
	]
	WORD	|__gcc_alloca_thread_free_all|
	DECLARE_FUNCTION __pthread_exit

	END
