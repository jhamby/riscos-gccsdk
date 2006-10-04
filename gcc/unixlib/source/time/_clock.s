; clock
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; clock_t clock (void)
	; Return the elapsed processor time. The base time is
	; arbitary, but doesn't change.
	EXPORT	clock
	NAME	clock
clock
	SWI	XOS_ReadMonotonicTime
	MOV	pc, lr

	END
