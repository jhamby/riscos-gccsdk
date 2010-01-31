@ __net_error
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ Function to record a networking (TCP/IP) SWI error.
	@ a1 is a (kernel_oserror *) RISC OS error block and errno is
	@ extracted from this.

	.global	__net_error
	NAME	__net_error
__net_error:
	STMFD	sp!, {a1, lr}
	MOV	a2, #0
	BL	__ul_seterr	   @ Make a copy of RISC OS error block
	LDMFD	sp!, {a1, lr}
	LDR	a1, [a1, #0]       @ Get real errno from RISC OS error number
	AND     a1, a1, #0xff      @ RISC OS 5 correctly returns error
	                           @ numbers in Internet error block.
	__set_errno	a1, a2
	MOV	pc, lr

	.end
