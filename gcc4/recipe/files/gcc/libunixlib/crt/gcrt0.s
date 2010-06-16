# Copyright (C) 2007-2010 UnixLib Developers

#include "internal/asm_dec.s"

	.file		"gcrt0.S"

	.global	_start
	.type	_start, %function
_start:
	@ On entry:
	@  a1 = Ptr to start of free memory
	@  a2 = Ptr to shared libraries initialisation function
	@  a3 = Ptr to shared libraries finalisation function
	@ These values are passed to us by the dynamic loader and stored
	@ in the data block which is then passed to the runtime library.
	LDR	a4, =crt1_data
	STR	a1, [a4, #CRT1_FREE_MEM]
	STR	a2, [a4, #CRT1_LIB_INIT]
	STR	a3, [a4, #CRT1_LIB_FINI]
	MOV	a1, a4
	
	@ On RISC OS the main entry point to the run-time library is
	@ always called __main.
	B	__main
	.size	_start, . - _start

	.data

crt1_data:
	.word	_init			@ #0
	.word	_fini			@ #4
	.word	__executable_start	@ #8
	.word	0			@ #12 - Free memory pointer
	.word	__data_start		@ #16
	.word	main			@ #20
	.word	1			@ #24 - Always 1 indicating that profiling should be enabled
	.word	0			@ #28 - Pointer to function that can be used to call all shared
					@	library static object ctors - can be NULL.
	.word	0			@ #32 - Pointer to function that can be used to call all shared
					@	library static object dtors - can be NULL.

# end of gcrt0.S
