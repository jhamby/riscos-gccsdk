# Copyright (C) 2007-2010 UnixLib Developers
	
	.file		"crt0.S"

	.global	_start
	.type	_start, %function
_start:
	@ On entry to _start, a1 is set by the dynamic loader to the start of
	@ free memory after it has claimed what it requires. This value is stored
	@ in the data block which is then passed to the runtime library.
	LDR	a2, =crt1_data
	STR	a1, [a2, #12]
	MOV	a1, a2
	
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
	.word	0			@ #24 - Always 0 indicating that profiling should be disabled.
	.word	0			@ #28 - Reserved for dynamic linker use
	.word	0			@ #32 - Reserved for dynamic linker use

# end of crt0.S
