@ __atomic_modify
@ Copyright (c) 2005-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ Atomically increment or decrement a word of memory
	.global	__atomic_modify
	NAME	__atomic_modify
__atomic_modify:
	@ a1 = address, a2 = amount to increment/decrement
	@ returns the new value
	SWI	XOS_IntOff
	LDR	a3, [a1]
	ADD	a3, a3, a2
	STR	a3, [a1]
	SWI	XOS_IntOn
	MOV	a1, a3
	MOV	pc, lr
	DECLARE_FUNCTION __atomic_modify

	.end
