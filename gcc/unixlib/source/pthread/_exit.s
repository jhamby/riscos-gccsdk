; Call alloca thread free functions as necessary
; Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__alloca_thread_free_all|, WEAK
	IMPORT	|___arm_alloca_thread_free_all|, WEAK
	IMPORT	|__gcc_alloca_thread_free_all|, WEAK

	EXPORT	|__pthread_exit|

	NAME	__pthread_exit
|__pthread_exit|
	STMFD	sp!, {lr}
	LDR	a1, funcs + 0
	TEQ	a1, #0
	MOVNE	lr, pc
	MOVNE	pc, a1

	LDR	a1, funcs + 4
	TEQ	a1, #0
	MOVNE	lr, pc
	MOVNE	pc, a1

	LDR	a1, funcs + 8
	TEQ	a1, #0
	MOVNE	lr, pc
	MOVNE	pc, a1

	LDMFD	sp!, {pc}

|funcs|
	DCD	|__alloca_thread_free_all|
	DCD	|___arm_alloca_thread_free_all|
	DCD	|__gcc_alloca_thread_free_all|

	END
