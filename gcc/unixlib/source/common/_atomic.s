; __atomic_modify
; Copyright (c) 2005-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; Atomically increment or decrement a word of memory
	EXPORT	|__atomic_modify|
	NAME	__atomic_modify
|__atomic_modify|
	; a1 = address, a2 = amount to increment/decrement
	; returns the new value
	SWI	XOS_IntOff
	LDR	a3, [a1]
	ADD	a3, a3, a2
	STR	a3, [a1]
	SWI	XOS_IntOn
	MOV	a1, a3
	MOV	pc, lr
	DECLARE_FUNCTION |__atomic_modify|

	END
